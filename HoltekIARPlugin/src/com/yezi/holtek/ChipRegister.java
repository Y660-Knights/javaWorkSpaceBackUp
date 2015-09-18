package com.yezi.holtek;

import java.util.ArrayList;
import java.util.List;

public class ChipRegister {
	private String name = null;
	
	private String zone = null;
	private int Bytesize = 0;
	private String Displaybase = null;
	
	private int offset = 0;
	private List<RegDomain> domains = new ArrayList<RegDomain>();
	
	//private StringBuffer domains = new StringBuffer();
	
	public ChipRegister (String name,int offset) {
		this.name = name;
		this.offset = offset;
		this.zone = "Memory";
		this.Bytesize = 4;
		this.Displaybase = "base=16";
	}
	
	public ChipRegister(String name, String offset) {
		int tba = Integer.parseUnsignedInt((offset.substring(2, offset.length())), 16);
		this.name = name;
		this.offset = tba;
		this.zone = "Memory";
		this.Bytesize = 4;
		this.Displaybase = "base=16";
	}

	public boolean addDomain(RegDomain rd) {
		//this.domains.insert(0, "\r\n\t__REG32\t\t" + domain);
		this.domains.add(0,rd);
		return true;
	}

	public String getName() {
		// TODO Auto-generated method stub
		return this.name;
	}

	public int getRegDomainSize() {
		// TODO Auto-generated method stub
		return this.domains.size();
	}

	public List<RegDomain> getRegDomains() {
		// TODO Auto-generated method stub
		return this.domains;
	}

	public int getOffset() {
		// TODO Auto-generated method stub
		return this.offset;
	}

	public String getZone() {
		return zone;
	}

	public int getBytesize() {
		return Bytesize;
	}

	public List<RegDomain> getDomains() {
		return domains;
	}

	public String getDisplaybase() {
		return Displaybase;
	}
}
