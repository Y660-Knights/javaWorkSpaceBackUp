package com.yezi.holtek;

import java.util.ArrayList;
import java.util.List;

public class ChipRegister {
	private String name = null;
	private String zone = null;
	private int Bytesize = 0;
	private int offset = 0;
	private List<RegDomain> domains = new ArrayList<RegDomain>();
	
	//private StringBuffer domains = new StringBuffer();
	
	public ChipRegister (String name,int offset) {
		this.name = name;
		this.offset = offset;
	}
	
	public boolean addDomain(RegDomain rd) {
		//this.domains.insert(0, "\r\n\t__REG32\t\t" + domain);
		return this.domains.add(rd);
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
}
