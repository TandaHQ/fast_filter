require 'spec_helper'
require 'tmpdir'

describe FastFilter do
  before(:all) do
    @arr = [10, 2, 100, 3, 1, 3]
  end

  it 'has a version number' do
    expect(FastFilter::VERSION).not_to be nil
  end

  describe ".filter_lt" do
    it 'should return values less than val on single value' do
      expect(FastFilter.filter_lt(@arr, 4)).to contain_exactly(1, 2, 3, 3)
    end

    it 'should return values less than val on double value' do
      expect(FastFilter.filter_lt(@arr, 3)).to contain_exactly(1, 2)
    end
  end

  describe ".filter_lte" do
    it 'should return values less than val on single value' do
      expect(FastFilter.filter_lte(@arr, 4)).to contain_exactly(1, 2, 3, 3)
    end

    it 'should return values less than val on double value' do
      expect(FastFilter.filter_lte(@arr, 3)).to contain_exactly(1, 2, 3, 3)
    end
  end

  describe ".filter_gt" do
    it 'should return values less than val on single value' do
      expect(FastFilter.filter_gt(@arr, 3)).to contain_exactly(10, 100)
    end

    it 'should return values less than val on double value' do
      expect(FastFilter.filter_gt(@arr, 2)).to contain_exactly(3, 3, 10, 100)
    end
  end

  describe ".filter_gte" do
    it 'should return values less than val on single value' do
      expect(FastFilter.filter_gte(@arr, 3)).to contain_exactly(3, 3, 10, 100)
    end

    it 'should return values less than val on double value' do
      expect(FastFilter.filter_gte(@arr, 2)).to contain_exactly(2, 3, 3, 10, 100)
    end
  end

  describe ".filter_lt_index" do
    it 'should return indices less than val on single value' do
      expect(FastFilter.filter_lt_index(@arr, 4)).to contain_exactly(1, 3, 4, 5)
    end

    it 'should return indices less than val on double value' do
      expect(FastFilter.filter_lt_index(@arr, 3)).to contain_exactly(1, 4)
    end
  end

  describe ".filter_lte_index" do
    it 'should return indices less than val on single value' do
      expect(FastFilter.filter_lte_index(@arr, 4)).to contain_exactly(1, 3, 4, 5)
    end

    it 'should return indices less than val on double value' do
      expect(FastFilter.filter_lte_index(@arr, 3)).to contain_exactly(1, 3, 4, 5)
    end
  end

  describe ".filter_gt_index" do
    it 'should return indices less than val on single value' do
      expect(FastFilter.filter_gt_index(@arr, 3)).to contain_exactly(0, 2)
    end

    it 'should return indices less than val on double value' do
      expect(FastFilter.filter_gt_index(@arr, 2)).to contain_exactly(0, 2, 3, 5)
    end
  end

  describe ".filter_gte_index" do
    it 'should return indices less than val on single value' do
      expect(FastFilter.filter_gte_index(@arr, 3)).to contain_exactly(0, 2, 3, 5)
    end

    it 'should return indices less than val on double value' do
      expect(FastFilter.filter_gte_index(@arr, 2)).to contain_exactly(0, 1, 2, 3, 5)
    end
  end
end
