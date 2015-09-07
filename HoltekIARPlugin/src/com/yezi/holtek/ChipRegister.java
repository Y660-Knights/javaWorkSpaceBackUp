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
	
	public String getTypedefText() {
		String text = "";		
/*		text += "typedef struct {\r\n";
		
		for(RegDomain ed : domains) {
			text += ("\t__REG32\t" + ed.getName() + "\t:\t" + (ed.getBitRangeEnd() - ed.getBitRangeStart() +1) + "\r\n");
		}
		if(this.module.length() == 0 || this.name.length() == 0)
			text += "\r\n} __" + this.module.toLowerCase() + this.name.toLowerCase() + "_bits\r\n\r\n";
		else
			text += "\r\n} __" + this.module.toLowerCase() + "_" + this.name.toLowerCase() + "_bits\r\n\r\n";*/
		
		return 	text;			
	}
	
	public String getDDFText() {
		String text = null;
		
		return text; 
	}
	
	public boolean addDomain(String domain,int bitRangeStart,int bitRangeEnd) {
		//this.domains.insert(0, "\r\n\t__REG32\t\t" + domain);
		return this.domains.add(new RegDomain(bitRangeStart, bitRangeEnd, domain));
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
}
