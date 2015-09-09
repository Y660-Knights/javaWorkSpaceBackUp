package com.yezi.holtek;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;
import java.util.Map.Entry;

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
				content += "sfr = \"" + cr.getName() + "\"\t\t\t, \"" + cr.getZone() + "\", " 
						+ "0x"+Integer.toHexString((cm.getBaseAddress() +cr.getOffset())).toUpperCase() + ", " + cr.getBytesize() + ", " 
						+ cr.getDisplaybase() + "\r\n";
				for(RegDomain rd : cr.getDomains()) {
					content += "sfr = \"" + cr.getName()+"." + rd.getName() + "\"\t, \"" + cr.getZone() + "\", " 
							+ "0x"+Integer.toHexString((cm.getBaseAddress() +cr.getOffset())).toUpperCase() + ", " + cr.getBytesize() + ", " 
							+ cr.getDisplaybase() + ", bitRange=" +rd.getBitRangeStart() +"-" + rd.getBitRangeEnd() + "\r\n";
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
		
		//interrupt
		content += "[InterruptList]\r\n";
		Properties p = new Properties();
		try {
			p.load(new FileInputStream(chip.getPath() + "\\dInterrupt.properties"));
			int i = 0;
			for(Entry e : p.entrySet()) { //Interrupt0   = NMI            0x008
				String temp = "";
				temp = "Interrupt" + i + "\t=\t" + e.getKey() + "\t\t0x" + Integer.toHexString((Integer.parseInt(e.getValue().toString()) * 4)).toUpperCase() + "\r\n";
				i++;
				content += temp;
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		fa = new FileAssistor(chip.getPath() + "\\debugger\\Holtek\\" + chip.getChipName() + ".ddf");
		fa.outputFile(content);
		content = "";
		return true;
	}

}
