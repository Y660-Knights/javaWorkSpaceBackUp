package com.yezi.holtek;

import java.io.File;

public class DdfFile implements HolteckPropertiesFile{
	private String content = "[Sfr]\r\n";
	private FileAssistor fa = null;
	
	@Override
	public boolean create(Chip chip) {
		// TODO Auto-generated method stub
		
		//检查目录是否存在 debugger\Holtek
		File dir = new File(chip.getPath()+ "\\debugger");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(chip.getPath() + "\\debugger\\Holtek");
		if (!dir.exists())
			dir.mkdirs();
	
		/*
		 * ;     Name                               Zone      Address     Bytesize  Displaybase Bitrange
		 * sfr = "ICTR"                           , "Memory", 0xE000E004,        4, base=16
			sfr = "ICTR.INTLINESNUM"               , "Memory", 0xE000E004,        4, base=16,    bitRange=0-4
		 * 
		 * */
		
		for(ChipModule cm : chip.getChipModules()) { //SFR
			//这里缺一个模块注释
			for(ChipRegister cr : cm.getChipRegisters()) {
				content += "sfr = \"" + cr.getName() + "\"\t, \"" + cr.getZone() + "\", " 
						+ Integer.toHexString((cm.getBaseAddress() +cr.getOffset())) + "," + cr.getBytesize() + "," + cr.getDisplaybase() + "\r\n";
				for(RegDomain rd : cr.getDomains()) {
					content += "sfr = \"" + cr.getName() + "\"\t, \"" + cr.getZone() + "\", " 
							+ Integer.toHexString((cm.getBaseAddress() +cr.getOffset())) + "," + cr.getBytesize() + "," + cr.getDisplaybase()
							+ "bitRange=" +rd.getBitRangeStart() + rd.getBitRangeEnd() +"-"+ "\r\n";
				}
			}			
		}
		//group
		//这里缺一个group注释
		content += "[SfrGroupInfo]\r\n"; 
		for(ChipModule cm : chip.getChipModules()) { //SFR
			//这里缺一个模块注释
			String temp = "group = \"" + cm.getName() + "\",\t";
			for(ChipRegister cr : cm.getChipRegisters()) {
				//"ICTR",
				temp += "\"" + cr.getName() + "\",";
			}
			temp = temp.substring(0, temp.length() - 1);
			temp += "\r\n";
			content += temp;
		}
		//[InterruptList],这个东西直接复制过去就好了吧。
		//这里缺一个group注释
		
		fa = new FileAssistor(chip.getPath() + "\\debugger\\Holtek\\" + chip.getChipName() + ".ddf");
		fa.outputFile(content);
		return false;
	}

}
