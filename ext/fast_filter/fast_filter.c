#include <ruby.h>
#include <stdlib.h>

VALUE FastFilter = Qnil;

// ============== HELPER FUNCTIONS ==============

static int asc_comp_fn(const void *a, const void *b) {
  return ((*(int **)a)[0] - (*(int **)b)[0]);
}

static int bin_lower_bound(long **a, int lo, int hi, long x) {
  if (lo > hi) {
    return lo;
  }

  int mid = lo + (hi - lo) / 2;
  if (a[mid][0] == x) {
    return bin_lower_bound(a, lo, mid - 1, x);
  } else if (a[mid][0] > x) {
    return bin_lower_bound(a, lo, mid - 1, x);
  } else {
    return bin_lower_bound(a, mid + 1, hi, x);
  }
}

static int bin_upper_bound(long **a, int lo, int hi, long x) {
  if (lo > hi) {
    return lo;
  }

  int mid = lo + (hi - lo) / 2;
  if (a[mid][0] == x) {
    return bin_upper_bound(a, mid + 1, hi, x);
  } else if (a[mid][0] > x) {
    return bin_upper_bound(a, lo, mid - 1, x);
  } else {
    return bin_upper_bound(a, mid + 1, hi, x);
  }
}

// ============== BUILD AND FREE FUNCTIONS ==============

static void build_c_arr(long ***c_arr, VALUE *rb_array, int *len) {
  *len = (int) rb_array_len(*rb_array);
  *c_arr = (long **) malloc(*len * sizeof(long *));
  for (int i = 0; i < *len; i++) {
    (*c_arr)[i] = (long *) malloc(2 * sizeof(long));
    (*c_arr)[i][0] = FIX2LONG(rb_ary_entry(*rb_array, i));
    (*c_arr)[i][1] = i;
  }

  qsort(*c_arr, *len, sizeof(long), asc_comp_fn);
}

static void free_c_arr(long **c_arr, int len) {
  for (int i = 0; i < len; i++) {
    free(c_arr[i]);
  }
  free(c_arr);
}

static void build_rb_arr(VALUE *r_arr, long **c_arr, int start, int finish, int index_mode) {
  *r_arr = rb_ary_new2(finish - start);
  int c_arr_index = index_mode;

  for (int ci = start, ri = 0; ci < finish; ci++, ri++) {
    rb_ary_store(*r_arr, ri, LONG2FIX(c_arr[ci][c_arr_index]));
  }
}

// ============== PRIVATE FILTER FUNCTIONS ==============
// Handles both index and value mode functions, called by
// public filter functions

static void lt_filter_with_mode(VALUE *input_rb_array, VALUE *max_value, int index_mode, VALUE *output_rb_array) {
  int len;
  long **c_arr;
  build_c_arr(&c_arr, input_rb_array, &len);

  int last = bin_lower_bound(c_arr, 0, len - 1, FIX2LONG(*max_value));
  build_rb_arr(output_rb_array, c_arr, 0, last, index_mode);
  free_c_arr(c_arr, len);
}

static void lte_filter_with_mode(VALUE *input_rb_array, VALUE *max_value, int index_mode, VALUE *output_rb_array) {
  int len;
  long **c_arr;
  build_c_arr(&c_arr, input_rb_array, &len);

  int last = bin_upper_bound(c_arr, 0, len - 1, FIX2LONG(*max_value));
  build_rb_arr(output_rb_array, c_arr, 0, last, index_mode);
  free_c_arr(c_arr, len);
}

static void gt_filter_with_mode(VALUE *input_rb_array, VALUE *max_value, int index_mode, VALUE *output_rb_array) {
  int len;
  long **c_arr;
  build_c_arr(&c_arr, input_rb_array, &len);

  int first = bin_upper_bound(c_arr, 0, len - 1, FIX2LONG(*max_value));
  build_rb_arr(output_rb_array, c_arr, first, len, index_mode);
  free_c_arr(c_arr, len);
}

static void gte_filter_with_mode(VALUE *input_rb_array, VALUE *max_value, int index_mode, VALUE *output_rb_array) {
  int len;
  long **c_arr;
  build_c_arr(&c_arr, input_rb_array, &len);

  int first = bin_lower_bound(c_arr, 0, len - 1, FIX2LONG(*max_value));
  build_rb_arr(output_rb_array, c_arr, first, len, index_mode);
  free_c_arr(c_arr, len);
}

// ============== PUBLIC FILTER FUNCTIONS ==============
// Calls to the private filter functions

static VALUE fast_filter_lt(VALUE self, VALUE rb_array, VALUE max_value) {
  VALUE output_rb_array;
  lt_filter_with_mode(&rb_array, &max_value, 0, &output_rb_array);
  return output_rb_array;
}

static VALUE fast_filter_lte(VALUE self, VALUE rb_array, VALUE max_value) {
  VALUE output_rb_array;
  lte_filter_with_mode(&rb_array, &max_value, 0, &output_rb_array);
  return output_rb_array;
}

static VALUE fast_filter_gt(VALUE self, VALUE rb_array, VALUE max_value) {
  VALUE output_rb_array;
  gt_filter_with_mode(&rb_array, &max_value, 0, &output_rb_array);
  return output_rb_array;
}

static VALUE fast_filter_gte(VALUE self, VALUE rb_array, VALUE max_value) {
  VALUE output_rb_array;
  gte_filter_with_mode(&rb_array, &max_value, 0, &output_rb_array);
  return output_rb_array;
}

static VALUE fast_filter_lt_index(VALUE self, VALUE rb_array, VALUE max_value) {
  VALUE output_rb_array;
  lt_filter_with_mode(&rb_array, &max_value, 1, &output_rb_array);
  return output_rb_array;
}

static VALUE fast_filter_lte_index(VALUE self, VALUE rb_array, VALUE max_value) {
  VALUE output_rb_array;
  lte_filter_with_mode(&rb_array, &max_value, 1, &output_rb_array);
  return output_rb_array;
}

static VALUE fast_filter_gt_index(VALUE self, VALUE rb_array, VALUE max_value) {
  VALUE output_rb_array;
  gt_filter_with_mode(&rb_array, &max_value, 1, &output_rb_array);
  return output_rb_array;
}

static VALUE fast_filter_gte_index(VALUE self, VALUE rb_array, VALUE max_value) {
  VALUE output_rb_array;
  gte_filter_with_mode(&rb_array, &max_value, 1, &output_rb_array);
  return output_rb_array;
}

void Init_fast_filter() {
  FastFilter = rb_define_module("FastFilter");
  rb_define_singleton_method(FastFilter, "filter_lt", fast_filter_lt, 2);
  rb_define_singleton_method(FastFilter, "filter_gt", fast_filter_gt, 2);
  rb_define_singleton_method(FastFilter, "filter_lte", fast_filter_lte, 2);
  rb_define_singleton_method(FastFilter, "filter_gte", fast_filter_gte, 2);
  rb_define_singleton_method(FastFilter, "filter_lt_index", fast_filter_lt_index, 2);
  rb_define_singleton_method(FastFilter, "filter_gt_index", fast_filter_gt_index, 2);
  rb_define_singleton_method(FastFilter, "filter_lte_index", fast_filter_lte_index, 2);
  rb_define_singleton_method(FastFilter, "filter_gte_index", fast_filter_gte_index, 2);
}
