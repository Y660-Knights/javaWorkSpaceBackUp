package com.yezi.holtek;

public class ChipRegister {
	private String name = null;
	private String module = null;
	private StringBuffer domains = new StringBuffer();
	
	public ChipRegister (String name,String module) {
		this.name = name;
		this.module = module;
	}
	
	public String getTypedefText() {
		String text = "";
		text += "typedef struct {\r\n";
		text += this.domains.toString();
		text += "\r\n} __" + this.module + "_" + this.name.toLowerCase() + "_bits\r\n";
		return 	text;			
	}
	
	public boolean addDomain(String domain) {
		this.domains.insert(0, "__REG32\t" + domain + "\r\n");
		return true;
	}
	
}
