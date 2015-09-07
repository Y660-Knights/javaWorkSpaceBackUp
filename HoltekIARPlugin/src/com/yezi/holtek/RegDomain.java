package com.yezi.holtek;

public class RegDomain {
	private int bitRangeStart = 0;
	private int bitRangeEnd = 0;
	private String name = null;
	
	public RegDomain (int bitRangeStart,int bitRangeEnd,String name) {
		this.bitRangeStart = bitRangeStart;
		this.bitRangeEnd = bitRangeEnd;
		this.name = name;
	}

	public int getBitRangeStart() {
		return bitRangeStart;
	}

	public int getBitRangeEnd() {
		return bitRangeEnd;
	}

	public String getName() {
		return name;
	}
	
}


