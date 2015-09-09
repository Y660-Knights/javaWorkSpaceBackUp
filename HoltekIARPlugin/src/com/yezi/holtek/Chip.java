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
/*
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
		System.out.println();*/
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

		int matchCount = 0,matchCount2 = 0;
		Pattern p1 = Pattern.compile("Register ?\\d? ?\\((\\w+)\\)");
		Matcher m1 = null;
		
/*		if(cm.getName().equals("SPI")) 
			System.out.println();		
		*/
		while (m.find()) {
			String line = m.group().trim();
			m1 = p1.matcher(line);
			if (m1.find()) {
				matchCount++;
			}
		}
		
		m = p.matcher(str);
		p1 = Pattern.compile("Register ?\\d? ?[–||-] ?\\(?(\\w+)\\)?");
	  //p1 = Pattern.compile("Register ?\\d? ?[–||-] ?\\(?(\\w+)\\)?");
		while (m.find()) {
			String line = m.group().trim();
			m1 = p1.matcher(line);
			if (m1.find()) {
				matchCount2++;
			}
		}
		
		if (matchCount > matchCount2) { // 最少有两个寄存器,说明该表达式正确
			p1 = Pattern.compile("Register ?\\d? ?\\((\\w+)\\)");
			// System.out.println("Reg count = " + matchCount);
		} else { // 更换表达式
			p1 = Pattern.compile("Register ?\\d? ?[–||-] ?\\(?(\\w+)\\)?");
		}
		
		m = p.matcher(str);
		while (m.find()) {
			// 去掉换行符等特殊符号
			String line = null;
			line = m.group().trim();
			if (line.length() < 6)
				continue;
			
/*			if(cm.getName().equals("GPTM")) 
				System.out.println();		
			*/
			{	//因为这个寄存器是在最开始，所以不用管
				// 1.5.1	EXTI Interrupt Configuration Register n – EXTICFGRn, n = 0 ~ 15
				Pattern pt = Pattern.compile("Register ?n ?– ?(\\w+)n, n = (\\d) ~ (\\d{0,3})");
				m1 = pt.matcher(line);
				
				if (m1.find()) {
					String namet = m1.group(1);
					int s, e, startOffset = 0;
					s = Integer.parseInt(m1.group(2));
					e = Integer.parseInt(m1.group(3));
					List<RegDomain> rds = new ArrayList<>();

					// 找offset
					while (m.find()) {
						line = m.group().trim();
						Pattern p3 = Pattern.compile("[o||O]ffset ?:? ?(\\w{5}) ?\\(?\\d?\\)? ?~ ?(\\w{5})");
						Matcher m3 = p3.matcher(line);
						if (m3.find()) {
							startOffset = Integer.parseInt(m3.group(1).substring(2, m3.group(1).length()), 16);
							break;
						}
					}
					// 找域名和域名长度
					while (m.find()) {						
						line = m.group().trim();
						if(line.length() < 6)
							continue;
						Pattern p2 = Pattern.compile("\\[(\\d{1,2}):?(\\d{0,2})\\](\\w+)");
						Matcher m2 = p2.matcher(line);

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
								start = Integer.parseInt(m2.group(2));
								end = Integer.parseInt(m2.group(1));
							}
							RegDomain rd = new RegDomain(start, end, domain);
							rds.add(rd);
						}
					}

					for (int i = s; i < (e - s + 1); i++) {
						ChipRegister crt = new ChipRegister(namet + i, startOffset + i * 4);
						for (RegDomain rd : rds) {
							String n = rd.getName();
							StringBuffer sbn = new StringBuffer(n);
							int t = n.indexOf('n');
							sbn.replace(t, t+1, "" + i);
							n = sbn.toString();
							RegDomain rdt = new RegDomain(rd.getBitRangeStart(), rd.getBitRangeEnd(), n + i);
							crt.addDomain(rdt);
						}
						cm.addChipRegister(crt);
					}
				}
				// 1.5.1	EXTI Interrupt Configuration Register n – EXTICFGRn, n = 0 ~ 15
			}
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
					
					Pattern p2 = Pattern.compile("\\[(\\d{1,2}):?(\\d{0,2})\\](\\w+)\\[?\\d{0,2}:?\\d{0,2}\\]??\\w* ?\\w* ?\\w* ?\\(?n? ?=? ?(\\d{0,2}) ?~? ?(\\d{0,2})");
					//Pattern p2 = Pattern.compile("\\[(\\d{1,2}):?(\\d{0,2})\\](\\w+)");
					Matcher m2 = p2.matcher(line);
					while (m2.find()) { //
						String domain;
						int start, end;
						domain = m2.group(3);
						if (m2.group(2).length() == 0) {
							start = Integer.parseInt(m2.group(1));
							end = start;
						} else {
							start = Integer.parseInt(m2.group(2));
							end = Integer.parseInt(m2.group(1));
						}

						/*
						 * for(int i = 0; i < m2.groupCount(); i++) {
						 * System.out.println(m2.group(i+1)); }
						 */
						//[31:0]	EXTInPIN[3:0] EXTIn Pin Selection (n = 0 ~ 7) 这种特殊域名的写法
						//if(domain.equals("EXTInPIN"))
						if(m2.group(4).length() > 0 && m2.group(5).length() > 0) { //说明是特殊写法
							int na = Integer.parseInt(m2.group(4));
							int nb = Integer.parseInt(m2.group(5));
							int i = nb;
							na = (nb - na + 1);
							nb = 32 / na;
							
							for(int j = 0; j < na; j ++) {
									String n = domain;
									StringBuffer sbn = new StringBuffer(n);									
									int t = n.indexOf('n');
									sbn.replace(t, t+1, "" + (i - j));
									n = sbn.toString();
									RegDomain rdt = new RegDomain(end - nb + 1, end, n);
									end -= nb;
									cr.addDomain(rdt);
							}
						} else {
							RegDomain rd = new RegDomain(start, end, domain);
							cr.addDomain(rd);								
						}
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
									start = Integer.parseInt(m2.group(2));
									end = Integer.parseInt(m2.group(1));
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
					
					
					// 如果是特殊的写法 Custom ID Register n (CIDRn, n = 0 ~3)
					pt = Pattern.compile("Register ?n ?\\((\\w+)n, n = (\\d) ~ ?(\\d)\\)");
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
							Pattern p3 = Pattern.compile("[o||O]ffset ?:? ?(\\w{5}) ?\\(?\\d?\\)? ?~ ?(\\w{5})");
							Matcher m3 = p3.matcher(line);
							if (m3.find()) {
								startOffset = Integer.parseInt(m3.group(1).substring(2, m3.group(1).length()), 16);
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
									start = Integer.parseInt(m2.group(2));
									end = Integer.parseInt(m2.group(1));
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
					// 如果是特殊的写法 Custom ID Register n (CIDRn, n = 0 ~3)
					
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
					/*for(RegDomain rdt : crt.getRegDomains()) {
						crt2.addDomain(rdt);
					}*/
					for(int i = 0; i < 8; i++) {
						String dn = "PACFG";
						RegDomain rdt = new RegDomain(32 - (i + 1) * 4 , 32 - i * 4 -1, dn + (7-i));
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);					
					name = "GPBCFGLR";
					crt2 = new ChipRegister(name, crt.getOffset() + 4);
					for(int i = 0; i < 8; i++) {
						String dn = "PBCFG";
						RegDomain rdt = new RegDomain(32 - (i + 1) * 4 , 32 - i * 4 -1, dn + (7-i));
						crt2.addDomain(rdt);
					}
/*					for(RegDomain rdt : crt.getRegDomains()) {
						crt2.addDomain(rdt);
					}*/
					cm.addChipRegister(crt2);
				}
				//处理AB 寄存器情况
				if(regName.equals("GPXCFGHR".toUpperCase())) {
					ChipRegister crt,crt2 = null;
					crt = cm.getChipRegisters().get(cm.getChipRegisterSize() -1);
					cm.getChipRegisters().remove(crt);
					String name = "GPACFGHR";
					crt2 = new ChipRegister(name, crt.getOffset());
/*					for(RegDomain rdt : crt.getRegDomains()) {
						crt2.addDomain(rdt);
					}*/
					for(int i = 0; i < 8; i++) {
						String dn = "PACFG";
						RegDomain rdt = new RegDomain(32 - (i + 1) * 4 , 32 - i * 4 -1, dn + ((7-i) + 8));
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);					
					name = "GPBCFGHR";
					crt2 = new ChipRegister(name, crt.getOffset() + 4);
/*					for(RegDomain rdt : crt.getRegDomains()) {
						crt2.addDomain(rdt);
					}*/
					for(int i = 0; i < 8; i++) {
						String dn = "PBCFG";
						RegDomain rdt = new RegDomain(32 - (i + 1) * 4 , 32 - i * 4 -1, dn + ((7-i) + 8));
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);
				}
			}
		}
	}

	public String getChipName() {
		return chipName;
	}

	public String getCodeRange() {
		return codeRange;
	}

	public String getRAMSize() {
		return RAMSize;
	}

	public String getBlockSize() {
		return BlockSize;
	}

	public List<ChipModule> getChipModules() {
		return chipModules;
	}

	public String getPath() {
		return path;
	}

}
