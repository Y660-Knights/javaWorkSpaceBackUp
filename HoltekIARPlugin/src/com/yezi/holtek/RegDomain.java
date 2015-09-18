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

	public RegDomain(String bitRangeStart, String bitRangeEnd, String name) {
		int t1 = Integer.parseUnsignedInt(bitRangeStart);
		int t2 = Integer.parseUnsignedInt(bitRangeEnd);
		this.bitRangeStart = t1;
		this.bitRangeEnd = t2;
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

	public int getBitRangeCount() {
		// TODO Auto-generated method stub
		return (this.bitRangeEnd - this.bitRangeStart + 1);
	}
	
}


