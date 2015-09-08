package com.yezi.holtek;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.output.Format;
import org.jdom2.output.XMLOutputter;

public class FlashFile implements HolteckPropertiesFile{
	
	@Override
	public boolean create(Chip chip) {
		// TODO Auto-generated method stub
		
		File dir = new File(chip.getPath() + "\\flashloader");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(chip.getPath() + "\\flashloader\\Holtek");
		if (!dir.exists())
			dir.mkdirs();
		
		// .flash
		Element flashRoot = new Element("flash_device");
		Document flashDoc = new Document(flashRoot);
		flashRoot.addContent(new Element("exe")
				.setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx_RAM" + chip.getRAMSize()+ "K.out"));
		flashRoot.addContent(new Element("page").setText("4"));
		flashRoot.addContent(new Element("block").setText(chip.getBlockSize() + " 0x400"));
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
					new FileOutputStream(chip.getPath() + "\\flashloader\\Holtek\\Flash" + chip.getChipName() + ".flash"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return true;
	}

}
