package com.yezi.holtek;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.ini4j.Wini;
import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.output.Format;
import org.jdom2.output.XMLOutputter;
import org.textmining.text.extraction.WordExtractor;

public class Chip {

	private String chipName = null;
	private String codeRange = null;
	private String RAMSize = "";
	private String BlockSize = "";
	private List<ChipModule> chipModules;
	private String path = null;

	public Chip(String path, String chipName, String RAMSize, String BlockSize) {
		// TODO Auto-generated constructor stub
		this.path = path;
		this.chipName = chipName;
		this.RAMSize = RAMSize;
		this.BlockSize = BlockSize;
		this.chipModules = new ArrayList<ChipModule>();

		this.loadChipModules();

		System.out.println(this.chipName + ":" + this.chipModules.size());
		for (ChipModule cm : chipModules) {
			System.out.println(" " + cm.getName() + ":" + cm.getChipRegisterSize());
			for (ChipRegister cr : cm.getChipRegisters()) {
				System.out.println("  " + cr.getName() + ":" + cr.getRegDomainSize());
				for (RegDomain rd : cr.getRegDomains()) {
					System.out.println("   " + rd.getName() + ":" + rd.getBitRangeStart() + "-" + rd.getBitRangeEnd());
				}
			}
		}
		System.out.println();
	}

	private void loadChipModules() {
		File file = new File(this.path);
		if (!file.isDirectory())
			return;

		for (File f : file.listFiles()) {
			if (!f.isDirectory()) {
				Pattern p = Pattern.compile("_([\\w&&[^_]]+).doc");
				Matcher m = p.matcher(f.getName());
				m.find();
				if (!m.find(0)) {
					continue;
				}
				Properties pro = new Properties();
				File prof = new File(this.path + "\\chip.properties");
				FileInputStream fis = null;
				try {
					fis = new FileInputStream(prof);
				} catch (FileNotFoundException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				try {
					pro.load(fis);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				String ba = pro.getProperty(m.group(1));
				// int tba =
				// Integer.parseInt(ba.substring(2,ba.length()).trim(),16);
				if (ba == null || ba.length() == 0) {
					continue;
				}
				int tba = Integer.parseUnsignedInt((ba.substring(2, ba.length())), 16);
				ChipModule cm = new ChipModule(m.group(1), f.getParent() + "\\" + f.getName(), tba);

				loadChipRegisters(cm);
				this.chipModules.add(cm);
			}
		}
	}

	private void loadChipRegisters(ChipModule cm) {
		// TODO Auto-generated method stub
		FileInputStream in = null;
		try {
			in = new FileInputStream(cm.getModulePath());
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		WordExtractor extractor = new WordExtractor();
		String str = null;
		try {
			str = extractor.extractText(in);
			in.close();
			// System.out.println(str);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		StringBuffer sb = new StringBuffer(str);
		StringBuffer sb2 = new StringBuffer("Register Descriptions");
		sb.reverse();
		sb2.reverse();
		int index = sb.indexOf(sb2.toString());
		if (index != -1) {
			sb2 = new StringBuffer(sb.substring(0, index));
			sb2.reverse();
			str = sb2.toString();
		} else
			return;

		// 获取doc中的一行
		Pattern p = Pattern.compile("\\r\\n.+");
		Matcher m = p.matcher(str);

		int matchCount = 0;
		Pattern p1 = Pattern.compile("Register ?\\d? ?\\((\\w+)\\)");
		Matcher m1 = null;

		while (m.find()) {
			String line = m.group().trim();
			m1 = p1.matcher(line);
			if (m1.find()) {
				matchCount++;
			}
		}

		/*
		 * if(cm.getName().equals("I2C")) System.out.println();
		 */

		if (matchCount > 2) { // 最少有两个寄存器,说明该表达式正确
			p1 = Pattern.compile("Register ?\\d? ?\\((\\w+)\\)");
			// System.out.println("Reg count = " + matchCount);
		} else { // 更换表达式
			p1 = Pattern.compile("Register ?\\d? ?– ?\\(?(\\w+)\\)?");
		}

		m = p.matcher(str);
		while (m.find()) {
			// 去掉换行符等特殊符号
			String line = null;
			line = m.group().trim();
			if (line.length() < 6)
				continue;
			m1 = p1.matcher(line);

			while (m1.find(0)) {// 找寄存器的名字
				// System.out.println(m1.group(1));
				String regName = "";
				int offset = 0;
				StringBuffer sb3 = new StringBuffer(m1.group(1));

				// 判断寄存器名字里面有没有包含模块名字,如果包含则删除
				int moduleIndex = sb3.indexOf(cm.getName());
				if (moduleIndex != -1)
					sb3.delete(moduleIndex, cm.getName().length());

				regName = sb3.toString().toUpperCase();
				

				// 找offset offset:0x008
				while (m.find()) {
					line = m.group().trim();
					Pattern p2 = Pattern.compile("[o||O]ffset {0,3}:{0,3}(\\w{5})");
					Matcher m2 = p2.matcher(line);
					if (m2.find()) {
						offset = Integer.parseInt(m2.group(1).substring(2, m2.group(1).length()), 16);
						break;
					}
				}
				
				ChipRegister cr = new ChipRegister(regName, offset);
				cm.addChipRegister(cr);
				boolean isSpecailRegFind = false;
				
				while (m.find()) {// 获取全文的下一行
					line = m.group().trim();
					// [29:28]URPCLK
					// 如果找到寄存器名，表示上面一个寄存器所有域名已经找到.
					m1 = p1.matcher(line);
					if (m1.find())
						break;

					Pattern p2 = Pattern.compile("\\[(\\d{1,2}):?(\\d{0,2})\\](\\w+)");
					Matcher m2 = p2.matcher(line);
					while (m2.find()) { //
						String domain;
						int start, end;
						domain = m2.group(3);
						if (m2.group(2).length() == 0) {
							start = Integer.parseInt(m2.group(1));
							end = start;
						} else {
							start = Integer.parseInt(m2.group(1));
							end = Integer.parseInt(m2.group(2));
						}
						RegDomain rd = new RegDomain(start, end, domain);
						cr.addDomain(rd);
						/*
						 * for(int i = 0; i < m2.groupCount(); i++) {
						 * System.out.println(m2.group(i+1)); }
						 */
					}

					// 如果是特殊的写法 ADC Conversion Data Register y – ADCDRy, y = 0 ~ 7
					Pattern pt = Pattern.compile("Register ?y ?– ?(\\w+)y, y = (\\d) ~ (\\d)");
					m1 = pt.matcher(line);
					if (m1.find()) {
						isSpecailRegFind = true;
						String namet = m1.group(1);
						int s, e, startOffset = 0;
						s = Integer.parseInt(m1.group(2));
						e = Integer.parseInt(m1.group(3));
						List<RegDomain> rds = new ArrayList<>();

						// 找offset
						while (m.find()) {
							line = m.group().trim();
							Pattern p3 = Pattern.compile("[o||O]ffset {0,3}:{0,3}(\\w{5}) {0,3}~ {0,3}(\\w{5})");
							Matcher m3 = p3.matcher(line);
							if (m3.find()) {

								startOffset = Integer.parseInt(m3.group(1).substring(2, m3.group(1).length()), 16);
								// endOffset =
								// Integer.parseInt(m3.group(2).substring(2,
								// m3.group(2).length()),16);
								break;
							}
						}
						// 找域名和域名长度
						while (m.find()) {
							line = m.group().trim();
							m2 = p2.matcher(line);

							// 如果找到寄存器名，表示上面一个寄存器所有域名已经找到.
							m1 = p1.matcher(line);
							if (m1.find())
								break;

							while (m2.find()) { //
								String domain;
								int start, end;
								domain = m2.group(3);
								if (m2.group(2).length() == 0) {
									start = Integer.parseInt(m2.group(1));
									end = start;
								} else {
									start = Integer.parseInt(m2.group(1));
									end = Integer.parseInt(m2.group(2));
								}
								RegDomain rd = new RegDomain(start, end, domain);
								rds.add(rd);
							}
						}

						for (int i = s; i < (e - s + 1); i++) {
							ChipRegister crt = new ChipRegister(namet + i, startOffset + i * 4);
							for (RegDomain rd : rds) {
								String n = rd.getName();
								n = n.substring(0, n.length() - 1);
								RegDomain rdt = new RegDomain(rd.getBitRangeStart(), rd.getBitRangeEnd(), n + i);
								crt.addDomain(rdt);
							}
							cm.addChipRegister(crt);
						}
					}
					// 如果是特殊的写法 ADC Conversion Data Register y – ADCDRy, y = 0 ~ 7
					
					if(isSpecailRegFind)
						break;
				}
				
				//处理AB 寄存器情况
				if(regName.equals("GPxCFGLR".toUpperCase())) {
					ChipRegister crt,crt2 = null;
					crt = cm.getChipRegisters().get(cm.getChipRegisterSize() -1);
					cm.getChipRegisters().remove(crt);
					String name = "GPACFGLR";
					crt2 = new ChipRegister(name, crt.getOffset());
					for(RegDomain rdt : crt.getRegDomains()) {
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);					
					name = "GPBCFGLR";
					crt2 = new ChipRegister(name, crt.getOffset() + 4);
					for(RegDomain rdt : crt.getRegDomains()) {
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);
				}
				//处理AB 寄存器情况
				if(regName.equals("GPXCFGHR".toUpperCase())) {
					ChipRegister crt,crt2 = null;
					crt = cm.getChipRegisters().get(cm.getChipRegisterSize() -1);
					cm.getChipRegisters().remove(crt);
					String name = "GPACFGHR";
					crt2 = new ChipRegister(name, crt.getOffset());
					for(RegDomain rdt : crt.getRegDomains()) {
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);					
					name = "GPBCFGHR";
					crt2 = new ChipRegister(name, crt.getOffset() + 4);
					for(RegDomain rdt : crt.getRegDomains()) {
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);
				}
				
				
				
			}

		}
	}

	public void outputIARPluginFile(File file) {

		createDevicesProperties(file);
		createFlashloaderProperties(file);
		createIncProperties(file);

	}

	private void createIncProperties(File file) {
		// TODO Auto-generated method stub
		// inc\Holtek
		File dir = new File(file.getPath() + "\\inc");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(file.getPath() + "\\inc\\Holtek");
		if (!dir.exists())
			dir.mkdirs();

		HFile chipHF = new HFile(file.getPath() + "\\inc\\Holtek");
		String moduleName = "";
		String modulePath = ""; // E:\\share\\IAR添加M0芯片型号\\Register Display
								// naming\\User manual\\HT32F52230_AFIO.doc

		for (File f : file.listFiles()) {
			if (!f.isDirectory()) {
				Pattern p = Pattern.compile("_([\\w&&[^_]]+).doc");
				Matcher m = p.matcher(f.getName());
				m.find();
				if (!m.find(0)) {
					continue;
				}

				modulePath = f.getPath();
				moduleName = m.group(1);
				// if(moduleName.length() > 10)
				// System.out.println(moduleName);
				loadChipRegisters(chipHF, moduleName, modulePath);
			}
		}
		chipHF.addInclude("io_macros");
		chipHF.addIfdef("#ifdef __IAR_SYSTEMS_ICC__\r\n#ifndef _SYSTEM_BUILD\r\n\t#pragma system_include\r\n#endif");
		chipHF.createFile("io" + this.chipName);
	}

	private void loadChipRegisters(HFile chipHF, String moduleName, String modulePath) {
		/*
		 * // TODO Auto-generated method stub FileInputStream in = null; try {
		 * in = new FileInputStream (modulePath); } catch (FileNotFoundException
		 * e) { // TODO Auto-generated catch block e.printStackTrace(); }
		 * WordExtractor extractor = new WordExtractor(); String str = null; try
		 * { str = extractor.extractText(in); in.close();
		 * //System.out.println(str); } catch (Exception e) { // TODO
		 * Auto-generated catch block e.printStackTrace(); } StringBuffer sb =
		 * new StringBuffer(str); StringBuffer sb2 = new StringBuffer(
		 * "Register Descriptions"); sb.reverse(); sb2.reverse(); int index =
		 * sb.indexOf(sb2.toString()); if(index != -1) { sb2 = new
		 * StringBuffer(sb.substring(0,index)); sb2.reverse(); str =
		 * sb2.toString(); }else return ;
		 * 
		 * //获取doc中的一行 Pattern p = Pattern.compile("\\r\\n.+"); Matcher m =
		 * p.matcher(str);
		 * 
		 * int matchCount = 0; Pattern p1 = Pattern.compile(
		 * "Register ?\\d? ?\\((\\w+)\\)");
		 * 
		 * while(m.find()) { String line = m.group().trim(); Matcher m1 =
		 * p1.matcher(line); if(m1.find()) { matchCount ++; } }
		 * 
		 * if(matchCount > 2) { //最少有两个寄存器,说明该表达式正确 p1 = Pattern.compile(
		 * "Register ?\\d? ?\\((\\w+)\\)"); //System.out.println("Reg count = "
		 * + matchCount); }else { //更换表达式 p1 = Pattern.compile(
		 * "Register ?\\d? ?– ?\\(?(\\w+)\\)?"); }
		 * 
		 * m = p.matcher(str); while(m.find()) { //去掉换行符等特殊符号 String line =
		 * m.group().trim(); // System.out.println(line); line =
		 * m.group().trim(); //这里的表达式分两种情况，有() 和 - //Pattern p1 =
		 * Pattern.compile("(Register ?\\d? ?–? ?)\\(?(\\w+)\\)?"); Matcher m1 =
		 * p1.matcher(line); if(m1.find()) {//找寄存器的名字
		 * //System.out.println(m1.group(1)); String regName = ""; StringBuffer
		 * sb3 = new StringBuffer(m1.group(1));
		 * 
		 * //判断寄存器名字里面有没有包含模块名字,如果包含则删除 int moduleIndex =
		 * sb3.indexOf(moduleName); if(moduleIndex != -1)
		 * sb3.delete(moduleIndex, moduleName.length() - 1);
		 * 
		 * regName = sb3.toString().toUpperCase(); ChipRegister cr = new
		 * ChipRegister(regName, moduleName,0x40000000); String address =
		 * "0x40000000"; //__IO_REG32_BIT(ICTR, 0xE000E004, __READ_WRITE ,
		 * __ictr_bits); if((regName + moduleName).length() > 15) address =
		 * "\t0x40000000"; else if((regName + moduleName).length() > 10) address
		 * = "\t\t0x40000000"; else if((regName + moduleName).length() > 6)
		 * address = "\t\t\t0x40000000"; else address = "\t\t\t\t0x40000000";
		 * 
		 * chipHF.addCall("__IO_REG32_BIT",moduleName + "_" +regName + "," +
		 * address + ", __READ_WRITE," + " __"+ moduleName.toLowerCase() + "_" +
		 * regName.toLowerCase()+"_bits"); while(m.find()) {//获取全文的下一行 line =
		 * m.group().trim(); if(line.matches(".+Offset.+") ||
		 * line.matches(".+offset.+")) { //如果本行含有 offset 说明是寄存器域名所在行
		 * //LPMODReserved Type/Reset Pattern p2 = Pattern.compile(
		 * "[\\w+ **]+Type/Reset"); Matcher m2 = p2.matcher(line);
		 * while(m2.find()) { //以Type/Reset为标识，把寄存器域名所在行分成一个一个的小单位，便于处理 String
		 * ts = m2.group(); ts = ts.replaceAll("Type/Reset", ""); Pattern p3 =
		 * Pattern.compile("( *{1,9})(\\w+)"); Matcher m3 = p3.matcher(ts);
		 * while(m3.find()) { //这里获得了域名,但是域名的长度无法获取。 if(m3.group(2).length() > 2
		 * && !m3.group(2).startsWith("0")) if(m3.group(2).equals("Reserved"))
		 * if(m3.group(2).length() <= 7) if(m3.group(2).length() <= 3)
		 * cr.addDomain("\t\t" + "\t\t\t:\t" + m3.group(1).length()); else
		 * cr.addDomain("\t\t" + "\t\t:\t" + m3.group(1).length()); else
		 * cr.addDomain("\t\t" + "\t:\t" + m3.group(1).length());
		 * cr.addDomain("", 0, 0); else if(m3.group(2).length() <= 7)
		 * if(m3.group(2).length() <= 3) cr.addDomain(m3.group(2) + "\t\t\t:\t"
		 * + m3.group(1).length()); else cr.addDomain(m3.group(2) + "\t\t:\t" +
		 * m3.group(1).length()); else cr.addDomain(m3.group(2) + "\t:\t" +
		 * m3.group(1).length()); cr.addDomain(m3.group(2), 0, 0); } } break; }
		 * } chipHF.addChipRegister(cr); } }
		 */
	}

	private void createFlashloaderProperties(File file) {
		// TODO Auto-generated method stub
		// .board
		File dir = new File(file.getPath() + "\\flashloader");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(file.getPath() + "\\flashloader\\Holtek");
		if (!dir.exists())
			dir.mkdirs();
		// .board flashloader\Holtek
		Element boardRoot = new Element("flash_board");
		Document boardDoc = new Document(boardRoot);
		Element boardpass = new Element("pass");
		Element boardpass2 = new Element("pass");

		boardpass.addContent(new Element("loader")
				.setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\Flash" + this.chipName + ".flash"));
		boardpass.addContent(new Element("range").setText("CODE 0x00000000 " + this.codeRange));

		boardpass2.addContent(new Element("loader")
				.setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\Flash" + this.chipName + "_op.flash"));
		boardpass2.addContent(new Element("range").setText("CODE 0x1ff00000 0x1ff003ff"));

		boardRoot.addContent(boardpass);
		boardRoot.addContent(boardpass2);
		// 输出 books.xml 文件；
		// 使xml文件 缩进效果
		Format boardFormat = Format.getPrettyFormat();
		XMLOutputter boardXMLOut = new XMLOutputter(boardFormat);
		try {
			// System.out.println(file.getPath());
			boardXMLOut.output(boardDoc,
					new FileOutputStream(file.getPath() + "\\flashloader\\Holtek\\Flash" + this.chipName + ".board"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		// .flash
		Element flashRoot = new Element("flash_device");
		Document flashDoc = new Document(flashRoot);
		flashRoot.addContent(new Element("exe")
				.setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx_RAM" + this.RAMSize + "K.out"));
		flashRoot.addContent(new Element("page").setText("4"));
		flashRoot.addContent(new Element("block").setText(this.BlockSize + " 0x400"));
		flashRoot.addContent(new Element("flash_base").setText("0x00000000"));
		flashRoot.addContent(
				new Element("macro").setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx.mac"));
		flashRoot.addContent(new Element("aggregate").setText("1"));
		// 输出 books.xml 文件；
		// 使xml文件 缩进效果
		Format flashFormat = Format.getPrettyFormat();
		XMLOutputter flashXMLOut = new XMLOutputter(flashFormat);
		try {
			// System.out.println(file.getPath());
			flashXMLOut.output(flashDoc,
					new FileOutputStream(file.getPath() + "\\flashloader\\Holtek\\Flash" + this.chipName + ".flash"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		// _op.flash
		Element flashopRoot = new Element("flash_device");
		Document flashopDoc = new Document(flashopRoot);
		flashopRoot.addContent(new Element("exe")
				.setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx_RAM" + this.RAMSize + "K.out"));
		flashopRoot.addContent(new Element("page").setText("4"));
		flashopRoot.addContent(new Element("block").setText(1 + " 0x400"));
		flashopRoot.addContent(new Element("flash_base").setText("0x00000000"));
		flashopRoot.addContent(
				new Element("macro").setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx.mac"));
		flashopRoot.addContent(new Element("aggregate").setText("1"));
		// 输出 books.xml 文件；
		// 使xml文件 缩进效果
		Format flashopFormat = Format.getPrettyFormat();
		XMLOutputter flashopXMLOut = new XMLOutputter(flashopFormat);
		try {
			// System.out.println(file.getPath());
			flashopXMLOut.output(flashopDoc, new FileOutputStream(
					file.getPath() + "\\flashloader\\Holtek\\Flash" + this.chipName + "_op.flash"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private boolean createDevicesProperties(File file) {
		// create directory
		File dir = new File(file.getPath() + "\\devices");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(file.getPath() + "\\devices\\Holtek");
		if (!dir.exists())
			dir.mkdirs();
		// .menu devices
		Element devicesRoot = new Element("optionMenuItem");
		Document devicesDoc = new Document(devicesRoot);
		devicesRoot.addContent(new Element("tag").setText(chipName));
		devicesRoot.addContent(new Element("display").setText("Holtek " + this.chipName));
		devicesRoot.addContent(new Element("data").setText("$CUR_DIR$\\" + this.chipName + ".i79"));
		// 输出 books.xml 文件；
		// 使xml文件 缩进效果
		Format devicesFormat = Format.getPrettyFormat();
		XMLOutputter devicesXMLOut = new XMLOutputter(devicesFormat);
		try {
			// System.out.println(file.getPath());
			devicesXMLOut.output(devicesDoc,
					new FileOutputStream(file.getPath() + "\\devices\\Holtek\\" + this.chipName + ".menu"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		// .i79
		String i79FileName = file.getPath() + "\\devices\\Holtek\\" + this.chipName + ".i79";
		try {
			File fIni = new File(i79FileName);
			OutputStream os = new FileOutputStream(fIni);
			os.close();
			Wini ini = new Wini(fIni);
			ini.put("FILEFORMAT", "rev", 1.6);

			ini.put("CHIP", "name", this.chipName);
			ini.put("CHIP", "endiansupport", "le");
			ini.put("CHIP", "thumbsupport", "true");
			ini.put("CHIP", "armsupport", "false");
			ini.put("CHIP", "fpu", "None");

			ini.put("CORE", "name", "Cortex-M3");

			ini.put("DDF FILE", "name", "Holtek\\" + this.chipName + ".ddf");

			ini.put("LINKER FILE", "name",
					"$TOOLKIT_DIR$\\config\\linker\\Holtek\\" + this.chipName.toLowerCase() + ".icf");

			ini.put("FLASH LOADER", "little",
					"$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\Flash" + this.chipName + ".board");

			ini.store();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return true;
	}
}
