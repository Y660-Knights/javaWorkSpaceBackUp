package com.yezi.holtek;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class HFile implements HolteckPropertiesFile{

	private FileAssistor fa;
	private String path = "";
	private String include = "";
	private String ifdef = "";
	private String call = "";
	private String register = "";
	private String interruptList = "";
	
	private String getContent() { //记得清空数据
		String content = "";
		content += this.include;
		this.include = "";
		
		content += this.ifdef;
		this.ifdef = "";
		
		content += this.register;
		this.register = "";
		
		content += "#endif\r\n\r\n";
		content += this.call + "\r\n";
		this.call = "";
		
		content += this.interruptList;
		this.interruptList = "";
		
		return content;
	}

	private void createFile(String name) {
		//if(this.fa == null)
		this.fa = new FileAssistor(path + "\\" + name + ".h");
		
		fa.outputFile("#ifndef\t__" + name.toUpperCase() + "_H\r\n"+ "#define\t__" + name.toUpperCase() + "_H\r\n\r\n" +this.getContent() + "\r\n#endif //__"+name.toUpperCase()+"_H"+"\r\n");
	}
	
	private void addInclude(String include) {
		this.include += "#include\t\"" + include + "\"\r\n\r\n";
	}

	private void addIfdef(String def) {
		// TODO Auto-generated method stub
		this.ifdef += "\r\n" + def + "\r\n\r\n";
	}

	private void addCall(String name, String parameter) {
		// TODO Auto-generated method stub
		this.call += name + "("+parameter+");\r\n";
	}

	@Override
	public boolean create(Chip chip) {
		// TODO Auto-generated method stub
		
		// inc\Holtek
		File dir = new File(chip.getPath() + "\\inc");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(chip.getPath() + "\\inc\\Holtek");
		if (!dir.exists())
			dir.mkdirs();
		
		this.path = chip.getPath() + "\\inc\\Holtek";
		
		this.addInclude("io_macros");
		this.addIfdef("#ifdef __IAR_SYSTEMS_ICC__\r\n#ifndef _SYSTEM_BUILD\r\n\t#pragma system_include\r\n#endif");
		
		for(ChipModule cm : chip.getChipModules()) {
			if(cm.getChipRegisterSize() < 1)
				continue;
			for(ChipRegister cr : cm.getChipRegisters()) {
				String temp = "typedef struct {\r\n";
				String regName = "";
				if(cm.getName().equals("OTHER")) {
					regName = cr.getName().toLowerCase();
				} else {
					regName = cm.getName().toLowerCase()+ "_" + cr.getName().toLowerCase();
				}
				int i = 0,count = 32,n = 0;
				for(RegDomain rd : cr.getDomains()) { //添加域名和域名长度,Reserved怎么解决呢？
					if(rd.getBitRangeStart() > i) { //说明有Reserved
						n = (rd.getBitRangeStart() - i);
						temp += "\t__REG32\t\t\t"+" : " +n+ ";\r\n";
						count -= n;
						i += n;
					}
					temp += "\t__REG32\t"+rd.getName()+" : " +rd.getBitRangeCount()+ ";\r\n";
					count-= rd.getBitRangeCount();
					i+= rd.getBitRangeCount();
				}
				if(count > 0) {
					temp += "\t__REG32\t\t\t"+" : " +count+ ";\r\n";
				}
				temp += "} __" + regName +"_bits;\r\n\r\n";
				this.register += temp;
				this.addCall("__IO_REG32_BIT", regName.toUpperCase() + ",\t\t0x"+Integer.toHexString((cm.getBaseAddress() + cr.getOffset())).toUpperCase()+", __READ_WRITE , " 
								+ regName + "_bits");
				
			}
		}
		//interrupt
		
		PropertiesUtil p = new PropertiesUtil();
		try {
			p.load(new FileInputStream(chip.getPath() + "\\hInterrupt.properties"));
			
			for(Object k : p.getKeyList()) {
				Object v = p.get(k);
				this.interruptList += "#define " + k.toString() + "\t" + v.toString() + "\r\n";
			}
			
/*			for(Entry e : p.entrySet()) {
				this.interruptList += "#define " + e.getKey() + "\t" + e.getValue() + "\r\n";
			}
			*/
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		this.createFile("io" + chip.getChipName());
		return false;
	}
}
