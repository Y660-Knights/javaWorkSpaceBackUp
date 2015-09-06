package com.yezi.holtek;

public class ChipRegister {
	private String name = null;
	private String module = null;
	private StringBuffer domains = new StringBuffer();
	
	public ChipRegister (String name,String module) {
		this.name = name;
		this.module = module;
		//this.module = "";
	}
	
	public String getTypedefText() {
		String text = "";		
		text += "typedef struct {";
		text += this.domains.toString();
		/*StringBuffer sb = new StringBuffer(this.name);
		sb.insert(this.module.length(), '_');*/
		//this.module = "";
		if(this.module.length() == 0 || this.name.length() == 0)
			text += "\r\n} __" + this.module.toLowerCase() + this.name.toLowerCase() + "_bits\r\n\r\n";
		else
			text += "\r\n} __" + this.module.toLowerCase() + "_" + this.name.toLowerCase() + "_bits\r\n\r\n";
		
		return 	text;			
	}
	
	public boolean addDomain(String domain) {
		this.domains.insert(0, "\r\n\t__REG32\t\t" + domain);
		return true;
	}
	
}
