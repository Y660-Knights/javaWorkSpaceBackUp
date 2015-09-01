package com.yezi.holtek;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import org.ini4j.Wini;
import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.output.Format;
import org.jdom2.output.XMLOutputter;

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
        
        
      //.board 
        dir = new File(file.getPath() + "\\flashloader");        
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
        
        /*        
		Element root = new Element("optionMenuItem");
		Document doc = new Document(root);
		root.addContent(new Element("tag").setText(chipName));
		root.addContent(new Element("display").setText("Holtek " + this.chipName));
		root.addContent(new Element("data").setText("$CUR_DIR$\\"+this.chipName +".i79"));		
		// 输出 books.xml 文件；    
        // 使xml文件 缩进效果  
        Format format = Format.getPrettyFormat();  
        XMLOutputter XMLOut = new XMLOutputter(format);  
        try {
        	System.out.println(file.getPath());
			XMLOut.output(doc, new FileOutputStream(file.getPath() + "\\devices\\Holtek\\" + this.chipName + ".menu"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        */       
        //.flash
		
	}
}
