package com.yezi.holtek;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.output.Format;
import org.jdom2.output.XMLOutputter;

public class OpflashFile implements HolteckPropertiesFile {

	@Override
	public boolean create(Chip chip) {
		// TODO Auto-generated method stub
		File dir = new File(chip.getPath() + "\\flashloader");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(chip.getPath() + "\\flashloader\\Holtek");
		if (!dir.exists())
			dir.mkdirs();
		
		// _op.flash
		Element flashopRoot = new Element("flash_device");
		Document flashopDoc = new Document(flashopRoot);
		flashopRoot.addContent(new Element("exe")
				.setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\FlashHT32Fxxxx_RAM" + chip.getRAMSize() + "K.out"));
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
					chip.getPath() + "\\flashloader\\Holtek\\Flash" + chip.getChipName() + "_op.flash"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return true;
	}

}
