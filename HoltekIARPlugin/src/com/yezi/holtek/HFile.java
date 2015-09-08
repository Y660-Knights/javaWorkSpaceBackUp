package com.yezi.holtek;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class HFile implements HolteckPropertiesFile{

	private FileAssistor fa;
	private String path = "";
	private String include = "";
	private String ifdef = "";
	private String call = "";
	private String register = "";
	private String getContent() {
		String content = "";
		content += this.include;
		content += this.ifdef;
/*		for(ChipRegister f : this.chipRegister) {
			content += f.getTypedefText();
		}*/
		content += register;
		content += "#endif\r\n\r\n";
		content += call;
		
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
				for(RegDomain rd : cr.getDomains()) {
					temp += "\t__REG32\t"+rd.getName()+" : " +rd.getBitRangeCount()+ ";\r\n";
				}
				temp += "} __"+cm.getName().toLowerCase()+ "_" + cr.getName().toLowerCase()+ "_bits;\r\n\r\n";
				this.register += temp;
				this.addCall("__IO_REG32_BIT", cm.getName() + ",\t\t"+Integer.toHexString((cm.getBaseAddress() + cr.getOffset()))+", __READ_WRITE , " + "__"+cm.getName().toLowerCase()+ "_" + cr.getName().toLowerCase()+ "_bits");
			}
		}
						
		this.createFile("io" + chip.getChipName());
		return false;
	}
}
