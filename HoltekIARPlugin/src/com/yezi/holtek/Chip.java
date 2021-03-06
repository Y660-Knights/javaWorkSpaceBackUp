package com.yezi.holtek;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.JDOMException;
import org.jdom2.input.SAXBuilder;
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
		codeRange = "0x" + Integer.toHexString((Integer.parseInt(BlockSize) * 1024) - 1);
		this.RAMSize = RAMSize;
		this.BlockSize = BlockSize;
		this.chipModules = new ArrayList<ChipModule>();

		this.loadChipModules();

	}

	private void loadChipModules() {
		File file = new File(this.path);
		if (file == null || !file.isDirectory())
			return;

		// 加载xml中的寄存器
		for (File file2 : new File(file.getParent()).listFiles()) {
			if (file2 == null || file2.getName().length() < 5)
				continue;
			if (!file2.isDirectory() && file2.getName()
					.substring(file2.getName().length() - 4, file2.getName().length()).toLowerCase().equals(".xml")) {
				ChipModule cm2 = null;
				try {
					SAXBuilder builder = new SAXBuilder();
					InputStream in = new FileInputStream(file2);
					Document doc = builder.build(in);
					// 获取根节点-模块名字
					Element root = doc.getRootElement();
					cm2 = new ChipModule(root.getName(), this.path, root.getAttributeValue("baseAddress"));

					for (Element e : root.getChildren()) {// 获取子节点-寄存器
						String name = e.getName();
						String offset = e.getAttributeValue("offset");
						ChipRegister cr = new ChipRegister(name, offset);
						for (Element e2 : e.getChildren()) {
							String start = e2.getAttributeValue("start");
							String end = e2.getAttributeValue("end");

							RegDomain rd = new RegDomain(start, end, e2.getName());
							cr.addDomain(rd);
						}
						cm2.addChipRegister(cr);
					}
					this.chipModules.add(cm2);
				} catch (JDOMException e) {
					e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}

		// 加载doc中的寄存器
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
		} catch (Exception e) {
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

		int matchCount = 0, matchCount2 = 0;
		Pattern p1 = Pattern.compile("Register ?\\d? ?\\((\\w+)\\)");
		Matcher m1 = null;

		/*
		 * if(cm.getName().equals("SPI")) System.out.println();
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
		// p1 = Pattern.compile("Register ?\\d? ?[–||-] ?\\(?(\\w+)\\)?");
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

			{ // 处理特殊寄存器写法的代码. EXTICFGRn 这种带有n的---------------开始----------
				// 1.5.1 EXTI Interrupt Configuration Register n – EXTICFGRn, n
				// = 0 ~ 15
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
						if (line.length() < 6)
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
							sbn.replace(t, t + 1, "" + i);
							n = sbn.toString();
							RegDomain rdt = new RegDomain(rd.getBitRangeStart(), rd.getBitRangeEnd(), n + i);
							crt.addDomain(rdt);
						}
						cm.addChipRegister(crt);
					}
				}
				// 1.5.1 EXTI Interrupt Configuration Register n – EXTICFGRn, n
				// = 0 ~ 15
			} // 处理特殊寄存器写法的代码. EXTICFGRn 这种带有n的 ---------------结束----------

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

				while (m.find()) {// 寻找寄存器中所有的域名
					line = m.group().trim();
					// [29:28]URPCLK
					// 如果找到寄存器名，表示上面一个寄存器所有域名已经找到.
					m1 = p1.matcher(line);
					if (m1.find())
						break;

					if (cm.getName().equals("GPIO")) { // 这里用来处理GPIO模块中n的写法。
						/* 我把所有的情况都弄出来先。
						 * 
						 *1. [15:12], [9], [7:0]                        PADIRn                 GPIO Port A pin n Direction Control Bits (n = 0 ~ 7, 9, 12 ~ 15)
						 *2. [31:24], [19:18], [15:0]                   PADVn[1:0]             GPIO Port A pin n Output Current Drive Selection Control Bits (n = 0 ~ 7, 9, 12 ~ 15)
						 *3. [31:16]									  PALKEY                 GPIO Port A Lock Key
						 *4. [31:24], [17:14],[9:0]					  PBDVn[1:0] 			 GPIO Port B pin n Output Current Drive Selection Control Bits (n = 0 ~ 4, 7 ~ 8, 12 ~ 15)
						 *5. [15:0]									    PADIRn				   GPIO Port A pin n Direction Control Bits (n = 0 ~ 15)
						 * */
						
						Pattern p2 = Pattern.compile(
								//    1            2                    3          4                      5         6
								"\\[(\\d{1,2}):?(\\d{0,2})\\],? ?\\[?(\\d{0,2}):?(\\d{0,2})\\]?,? ?\\[?(\\d{0,2}):?(\\d{0,2})\\]?? ?"
								//   7            8         9   
								+ "(\\w*)\\[?(\\d{0,2}):?(\\d{0,2})\\]??[\\w|| ||-]*"
								//                 10            11              12             13            14              15           
								+ "\\(?n? ?=? ?(\\d{0,2}) ?~? ?(\\d{0,2}),? ?(\\d{0,2}) ?~? ?(\\d{0,2}),? ?(\\d{0,2}) ?~? ?(\\d{0,2})\\)?"
								);
						
						Matcher m2 = p2.matcher(line);
						while (m2.find()) { //
							if(m2.group(3).length() == 0) { 
								String domain;
								int start, end;
								domain = m2.group(7);
								
								if(m2.group(10).length() > 0) { //情况5
									int start1;
									domain = domain.substring(0, domain.length() - 1);
									
									start1 = Integer.parseInt(m2.group(1).length()==0?"0":m2.group(1));
									
									int n11, n12;
									n11 = Integer.parseInt(m2.group(10).length() == 0 ? "0" : m2.group(10));
									n12 = Integer.parseInt(m2.group(11).length() == 0 ? "0" : m2.group(11));
									
									int w11;
									w11 = Integer.parseInt(m2.group(8).length() == 0 ? "0" : m2.group(8));
									if(w11== 0) {
										for(int i = n12; i >= n11; i--) {
											start = start1--;
											end = start;
											
											RegDomain rd = new RegDomain(start, end, domain + i);
											cr.addDomain(rd);
										}
									} else {
										for(int i = n12; i >= n11; i--) {
											start = start1-1;
											end = start1;
											start1 -= 2;
											RegDomain rd = new RegDomain(start, end, domain + i);
											cr.addDomain(rd);
										}
									}
									
								}else {
									if (m2.group(2).length() == 0) { //说明是情况3，直接添加一个域就可以了。
										start = Integer.parseInt(m2.group(1));
										end = start;
									} else {
										start = Integer.parseInt(m2.group(2));
										end = Integer.parseInt(m2.group(1));
									}
									
									RegDomain rd = new RegDomain(start, end, domain);
									cr.addDomain(rd);
								}
/*								if( cr.getName().equals("PBDIRCR") )
									System.out.println();*/
/*								if( domain.equals("PADIRn") )
									System.out.println();*/
							} else {
								String domain;
								int start, end;
								
								domain = m2.group(7);
								
/*								if( domain.equals("PADIRn") )
									System.out.println();*/
								
								if(domain.length() == 0)
									continue;
								
								domain = domain.substring(0, domain.length() - 1);
								
								int start1, start2, start3;
								start1 = Integer.parseInt(m2.group(1).length()==0?"0":m2.group(1));
								start2 = Integer.parseInt(m2.group(3).length()==0?"0":m2.group(3));
								start3 = Integer.parseInt(m2.group(5).length()==0?"0":m2.group(5));
								
								int n11, n12,n21, n22,n31, n32;
								n11 = Integer.parseInt(m2.group(10).length() == 0 ? "0" : m2.group(10));
								n12 = Integer.parseInt(m2.group(11).length() == 0 ? "0" : m2.group(11));
								n21 = Integer.parseInt(m2.group(12).length() == 0 ? "0" : m2.group(12));
								n22 = Integer.parseInt(m2.group(13).length() == 0 ? "0" : m2.group(13));
								n31 = Integer.parseInt(m2.group(14).length() == 0 ? "0" : m2.group(14));
								n32 = Integer.parseInt(m2.group(15).length() == 0 ? "0" : m2.group(15));
								
								if(n22 == 0)
									n22 = n21;
								
								int w11;
								w11 = Integer.parseInt(m2.group(8).length() == 0 ? "0" : m2.group(8));
								
								/* 我把所有的情况都弄出来先。
								 * 
								 *1. [15:12], [9], [7:0]                        PADIRn                 GPIO Port A pin n Direction Control Bits (n = 0 ~ 7, 9, 12 ~ 15)
								 *2. [31:24], [19:18], [15:0]                   PADVn[1:0]             GPIO Port A pin n Output Current Drive Selection Control Bits (n = 0 ~ 7, 9, 12 ~ 15)
								 *3. [31:16]									PALKEY                 GPIO Port A Lock Key
								 *4. [31:24], [17:14],[9:0]					    PBDVn[1:0] 			   GPIO Port B pin n Output Current Drive Selection Control Bits (n = 0 ~ 4, 7 ~ 8, 12 ~ 15)
								 *5. [15:0]									    PADIRn				   GPIO Port A pin n Direction Control Bits (n = 0 ~ 15)
								 * 
								 * */
								if(w11 == 0) { //是情况1
									for(int i = n32; i >= n31; i--) {
										start = start1--;
										end = start;
										
										RegDomain rd = new RegDomain(start, end, domain + i);
										cr.addDomain(rd);
									}
									
									for(int i = n22; i >= n21; i--) {
										start = start2--;
										end = start;
										RegDomain rd = new RegDomain(start, end, domain + i);
										cr.addDomain(rd);
									}
									
									for(int i = n12; i >= n11; i--) {
										start = start3--;
										end = start;
										RegDomain rd = new RegDomain(start, end, domain + i);
										cr.addDomain(rd);
									}
								}
								
								if(w11 == 1) { //情況 2 4
									for(int i = n32; i >= n31; i--) {
										start = start1-1;
										end = start1;
										start1 -= 2;
										RegDomain rd = new RegDomain(start, end, domain + i);
										cr.addDomain(rd);
									}
									
									for(int i = n22; i >= n21; i--) {
										start = start2-1;
										end = start2;
										start2 -= 2;
										RegDomain rd = new RegDomain(start, end, domain + i);
										cr.addDomain(rd);
									}
									
									for(int i = n12; i >= n11; i--) {
										start = start3-1;
										end = start3;
										start3 -= 2;
										RegDomain rd = new RegDomain(start, end, domain + i);
										cr.addDomain(rd);
									}
								}
							}
						}
					} else { //其他非GPIO的情况
						Pattern p2 = Pattern.compile(
								"\\[(\\d{1,2}):?(\\d{0,2})\\](\\w+)\\[?\\d{0,2}:?\\d{0,2}\\]??\\w* ?\\w* ?\\w* ?\\(?n? ?=? ?(\\d{0,2}) ?~? ?(\\d{0,2})");
						// Pattern p2 =
						// Pattern.compile("\\[(\\d{1,2}):?(\\d{0,2})\\](\\w+)");
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

							// [31:0] EXTInPIN[3:0] EXTIn Pin Selection (n = 0 ~
							// 7)
							// 这种特殊域名的写法
							if (m2.group(4).length() > 0 && m2.group(5).length() > 0) { // 说明是特殊写法
								int na = Integer.parseInt(m2.group(4));
								int nb = Integer.parseInt(m2.group(5));
								int i = nb;
								na = (nb - na + 1);
								nb = 32 / na;

								for (int j = 0; j < na; j++) {
									String n = domain;
									StringBuffer sbn = new StringBuffer(n);
									int t = n.indexOf('n');
									sbn.replace(t, t + 1, "" + (i - j));
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

						// 如果是特殊的写法 ADC Conversion Data Register y – ADCDRy, y =
						// 0 ~ 7 ------开始--------------
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
						// 如果是特殊的写法 ADC Conversion Data Register y – ADCDRy, y =
						// 0 ~ 7
						// ------结束-------------------------------------------------------------------------

						// 如果是特殊的写法 Custom ID Register n (CIDRn, n = 0 ~3)
						// ------开始--------------
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
						// ------结束-------------------------------------------------------------------------

						if (isSpecailRegFind)
							break;
					}
				}
				// 1处理AB 寄存器情况 ------开始--------------
				if (regName.equals("GPxCFGLR".toUpperCase())) {
					ChipRegister crt, crt2 = null;
					crt = cm.getChipRegisters().get(cm.getChipRegisterSize() - 1);
					cm.getChipRegisters().remove(crt);
					String name = "GPACFGLR";
					crt2 = new ChipRegister(name, crt.getOffset());
					/*
					 * for(RegDomain rdt : crt.getRegDomains()) {
					 * crt2.addDomain(rdt); }
					 */
					for (int i = 0; i < 8; i++) {
						String dn = "PACFG";
						RegDomain rdt = new RegDomain(32 - (i + 1) * 4, 32 - i * 4 - 1, dn + (7 - i));
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);
					name = "GPBCFGLR";
					crt2 = new ChipRegister(name, crt.getOffset() + 4);
					for (int i = 0; i < 8; i++) {
						String dn = "PBCFG";
						RegDomain rdt = new RegDomain(32 - (i + 1) * 4, 32 - i * 4 - 1, dn + (7 - i));
						crt2.addDomain(rdt);
					}
					/*
					 * for(RegDomain rdt : crt.getRegDomains()) {
					 * crt2.addDomain(rdt); }
					 */
					cm.addChipRegister(crt2);
				} // 1处理AB 寄存器情况
					// ------结束-------------------------------------------------------------------------

				// 2处理AB 寄存器情况 ------开始--------------
				if (regName.equals("GPXCFGHR".toUpperCase())) {
					ChipRegister crt, crt2 = null;
					crt = cm.getChipRegisters().get(cm.getChipRegisterSize() - 1);
					cm.getChipRegisters().remove(crt);
					String name = "GPACFGHR";
					crt2 = new ChipRegister(name, crt.getOffset());
					/*
					 * for(RegDomain rdt : crt.getRegDomains()) {
					 * crt2.addDomain(rdt); }
					 */
					for (int i = 0; i < 8; i++) {
						String dn = "PACFG";
						RegDomain rdt = new RegDomain(32 - (i + 1) * 4, 32 - i * 4 - 1, dn + ((7 - i) + 8));
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);
					name = "GPBCFGHR";
					crt2 = new ChipRegister(name, crt.getOffset() + 4);
					/*
					 * for(RegDomain rdt : crt.getRegDomains()) {
					 * crt2.addDomain(rdt); }
					 */
					for (int i = 0; i < 8; i++) {
						String dn = "PBCFG";
						RegDomain rdt = new RegDomain(32 - (i + 1) * 4, 32 - i * 4 - 1, dn + ((7 - i) + 8));
						crt2.addDomain(rdt);
					}
					cm.addChipRegister(crt2);
				} // 2处理AB 寄存器情况------结束---------------------------------------------------------------
				
				if(cm.getName().equals("EXTI")) {
					RegDomain rd = cr.getRegDomains().get(0);
					if(rd.getName().indexOf("n") != -1) {
						String name = rd.getName();
						int s,e;
						e = rd.getBitRangeEnd();
						s = e;
						for(int i = 15; i >= 0; i--) {
							String nt = name.substring(0, name.indexOf("n")) + i+name.substring(name.indexOf("n") +1 ,name.length());
							RegDomain rdt = new RegDomain(s, e, nt);
							e -- ;
							s = e;
							cr.addDomain(rdt);
						}
						cr.getDomains().remove(rd);
					}
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
