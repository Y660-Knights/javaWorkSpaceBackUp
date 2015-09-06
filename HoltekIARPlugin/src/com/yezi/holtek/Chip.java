package com.yezi.holtek;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
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
	
	public Chip(String chipName,String RAMSize,String BlockSize) {
		// TODO Auto-generated constructor stub
		this.chipName = chipName;
		this.RAMSize = RAMSize;
		this.BlockSize = BlockSize;
	}

	public void outputIARPluginFile(File file) {		

		createDevicesProperties(file);
		createFlashloaderProperties(file);
		createIncProperties(file);
        
	}

	private void createIncProperties(File file) {
		// TODO Auto-generated method stub
	       //inc\Holtek
        File dir = new File(file.getPath() + "\\inc");        
		if(!dir.exists())
			dir.mkdirs();
		dir = new File(file.getPath() + "\\inc\\Holtek");
		if(!dir.exists())
			dir.mkdirs();
		
        HFile chipHF = new HFile(file.getPath() + "\\inc\\Holtek");
        String moduleName = "";
        String modulePath = ""; //E:\\share\\IAR添加M0芯片型号\\Register Display naming\\User manual\\HT32F52230_AFIO.doc
        
        for(File f : file.listFiles()) {
        	if(!f.isDirectory()) {
        		Pattern p = Pattern.compile(" ?_?(\\w+).doc");
        		Matcher m = p.matcher(f.getName());
        		while(m.find()) {
        			modulePath = f.getPath();
        			moduleName = m.group(1).toLowerCase();
        			loadChipRegisters(chipHF,moduleName,modulePath);
        		}
        	}
        }
        chipHF.createFile("io" + this.chipName);
	}
	
	

	private void loadChipRegisters(HFile chipHF, String moduleName, String modulePath) {
		// TODO Auto-generated method stub
		FileInputStream in = null;
		try {
			in = new FileInputStream (modulePath);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		WordExtractor extractor = new WordExtractor();
		String str = null;
		try {			
			str = extractor.extractText(in);
			in.close();
			//System.out.println(str);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		StringBuffer sb = new StringBuffer(str);
		StringBuffer sb2 = new StringBuffer("Register Descriptions");
		sb.reverse();
		sb2.reverse();
		int index = sb.indexOf(sb2.toString());
		if(index != -1) {
			sb2 = new StringBuffer(sb.substring(0,index));
			sb2.reverse();
			str = sb2.toString();
		}else
			return ;
			
		//获取doc中的一行
		Pattern p = Pattern.compile("\\r\\n.+");
		Matcher m = p.matcher(str);
		
		int matchCount = 0;		
		Pattern p1 = Pattern.compile("Register ?\\d? ?\\((\\w+)\\)");
		
		while(m.find()) {
			String line = m.group().trim();			
			Matcher m1 = p1.matcher(line);	
			if(m1.find()) {
				matchCount ++;
			}			
		}
		
		if(matchCount > 2) { //最少有两个寄存器,说明该表达式正确
			p1 = Pattern.compile("Register ?\\d? ?\\((\\w+)\\)");
			//System.out.println("Reg count = " + matchCount); 
		}else { //更换表达式
			p1 = Pattern.compile("Register ?\\d? ?– ?\\(?(\\w+)\\)?");
		}
		
		m = p.matcher(str);
		while(m.find()) {
			//去掉换行符等特殊符号
			String line = m.group().trim();			
	//		System.out.println(line);
			line = m.group().trim();
			//这里的表达式分两种情况，有()  和 - 
			//Pattern p1 = Pattern.compile("(Register ?\\d? ?–? ?)\\(?(\\w+)\\)?");
			Matcher m1 = p1.matcher(line);
			if(m1.find()) {//找寄存器的名字				
				//System.out.println(m1.group(1));
				ChipRegister cr = new ChipRegister(m1.group(1), moduleName);
				while(m.find()) {//获取全文的下一行
					line = m.group().trim();		
					if(line.matches(".+Offset.+") || line.matches(".+offset.+")) { //如果本行含有 offset 说明是寄存器域名所在行
						//LPMODReserved Type/Reset 
						Pattern p2 = Pattern.compile("[\\w+ **]+Type/Reset");
						Matcher m2 = p2.matcher(line);	
						while(m2.find()) { //以Type/Reset为标识，把寄存器域名所在行分成一个一个的小单位，便于处理
							String ts = m2.group();
							ts = ts.replaceAll("Type/Reset", "");
							Pattern p3 = Pattern.compile("( *{1,9})(\\w+)");
							Matcher m3 = p3.matcher(ts);
							while(m3.find()) { //这里获得了域名,但是域名的长度无法获取。
								if(m3.group(2).length() > 2 &&  !m3.group(2).startsWith("0"))
									if(m3.group(2).equals("Reserved"))
										if(m3.group(2).length() <= 7)
											if(m3.group(2).length() <= 3)
												cr.addDomain("\t\t" + "\t\t\t:\t" + m3.group(1).length());
											else
												cr.addDomain("\t\t" + "\t\t:\t" + m3.group(1).length());
										else
											cr.addDomain("\t\t" + "\t:\t" + m3.group(1).length());
									else
										if(m3.group(2).length() <= 7)
											if(m3.group(2).length() <= 3)
												cr.addDomain(m3.group(2) + "\t\t\t:\t" + m3.group(1).length());
											else
												cr.addDomain(m3.group(2) + "\t\t:\t" + m3.group(1).length());
										else
											cr.addDomain(m3.group(2) + "\t:\t" + m3.group(1).length());
							}
						}
						break;
					}	
				}
				chipHF.addChipRegister(cr);
			}
		}
	}

	private void createFlashloaderProperties(File file) {
		// TODO Auto-generated method stub
	      //.board 
        File dir = new File(file.getPath() + "\\flashloader");        
		if(!dir.exists())
			dir.mkdirs();
		dir = new File(file.getPath() + "\\flashloader\\Holtek");
		if(!dir.exists())
			dir.mkdirs();
        //.board flashloader\Holtek
		Element boardRoot = new Element("flash_board");
		Document boardDoc = new Document(boardRoot);
		Element boardpass = new Element("pass");
		Element boardpass2 = new Element("pass");
		
		boardpass.addContent(new Element("loader").setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\Flash"+this.chipName+ ".flash"));
		boardpass.addContent(new Element("range").setText("CODE 0x00000000 " + this.codeRange));
		
		boardpass2.addContent(new Element("loader").setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\Flash"+this.chipName+ "_op.flash"));
		boardpass2.addContent(new Element("range").setText("CODE 0x1ff00000 0x1ff003ff"));
		
		boardRoot.addContent(boardpass);
		boardRoot.addContent(boardpass2);
		// 输出 books.xml 文件；    
        // 使xml文件 缩进效果  
        Format boardFormat = Format.getPrettyFormat();  
        XMLOutputter boardXMLOut = new XMLOutputter(boardFormat);  
        try {
        	//System.out.println(file.getPath());
			boardXMLOut.output(boardDoc, new FileOutputStream(file.getPath() + "\\flashloader\\Holtek\\Flash" + this.chipName + ".board"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
        //.flash               
		Element flashRoot = new Element("flash_device");
		Document flashDoc = new Document(flashRoot);
		flashRoot.addContent(new Element("exe").setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx_RAM"+this.RAMSize+"K.out"));
		flashRoot.addContent(new Element("page").setText("4"));
		flashRoot.addContent(new Element("block").setText(this.BlockSize + " 0x400"));
		flashRoot.addContent(new Element("flash_base").setText("0x00000000"));
		flashRoot.addContent(new Element("macro").setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx.mac"));
		flashRoot.addContent(new Element("aggregate").setText("1"));		
		// 输出 books.xml 文件；    
        // 使xml文件 缩进效果  
        Format flashFormat = Format.getPrettyFormat();  
        XMLOutputter flashXMLOut = new XMLOutputter(flashFormat);  
        try {
        	//System.out.println(file.getPath());
			flashXMLOut.output(flashDoc, new FileOutputStream(file.getPath() + "\\flashloader\\Holtek\\Flash" + this.chipName + ".flash"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
        //_op.flash               
		Element flashopRoot = new Element("flash_device");
		Document flashopDoc = new Document(flashopRoot);
		flashopRoot.addContent(new Element("exe").setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx_RAM"+this.RAMSize+"K.out"));
		flashopRoot.addContent(new Element("page").setText("4"));
		flashopRoot.addContent(new Element("block").setText(1 + " 0x400"));
		flashopRoot.addContent(new Element("flash_base").setText("0x00000000"));
		flashopRoot.addContent(new Element("macro").setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx.mac"));
		flashopRoot.addContent(new Element("aggregate").setText("1"));		
		// 输出 books.xml 文件；    
        // 使xml文件 缩进效果  
        Format flashopFormat = Format.getPrettyFormat();  
        XMLOutputter flashopXMLOut = new XMLOutputter(flashopFormat);  
        try {
        	//System.out.println(file.getPath());
        	flashopXMLOut.output(flashopDoc, new FileOutputStream(file.getPath() + "\\flashloader\\Holtek\\Flash" + this.chipName + "_op.flash"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private boolean createDevicesProperties(File file) {
		//create directory
		File dir = new File(file.getPath() + "\\devices");
		if(!dir.exists())
			dir.mkdirs();
		dir = new File(file.getPath() + "\\devices\\Holtek");
		if(!dir.exists())
			dir.mkdirs();
		//.menu devices
		Element devicesRoot = new Element("optionMenuItem");
		Document devicesDoc = new Document(devicesRoot);
		devicesRoot.addContent(new Element("tag").setText(chipName));
		devicesRoot.addContent(new Element("display").setText("Holtek " + this.chipName));
		devicesRoot.addContent(new Element("data").setText("$CUR_DIR$\\"+this.chipName +".i79"));		
		// 输出 books.xml 文件；    
        // 使xml文件 缩进效果  
        Format devicesFormat = Format.getPrettyFormat();  
        XMLOutputter devicesXMLOut = new XMLOutputter(devicesFormat);  
        try {
        	//System.out.println(file.getPath());
			devicesXMLOut.output(devicesDoc, new FileOutputStream(file.getPath() + "\\devices\\Holtek\\" + this.chipName + ".menu"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
        //.i79
        String i79FileName = file.getPath() + "\\devices\\Holtek\\" + this.chipName + ".i79";
        try {
        	File fIni = new File(i79FileName);        	
        	OutputStream os = new FileOutputStream(fIni);
        	os.close();
			Wini ini = new Wini(fIni);
			ini.put("FILEFORMAT", "rev",1.6);
			
			ini.put("CHIP", "name",this.chipName);
			ini.put("CHIP", "endiansupport","le");
			ini.put("CHIP", "thumbsupport","true");
			ini.put("CHIP", "armsupport","false");
			ini.put("CHIP", "fpu","None");
			
			ini.put("CORE", "name","Cortex-M3");
			
			ini.put("DDF FILE", "name","Holtek\\"+this.chipName+".ddf");
			
			ini.put("LINKER FILE", "name","$TOOLKIT_DIR$\\config\\linker\\Holtek\\"+this.chipName.toLowerCase()+".icf");
			
			ini.put("FLASH LOADER", "little","$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\Flash"+this.chipName+".board");
			
			ini.store();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return true;
	}
}
