package com.yezi.holtek;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.output.Format;
import org.jdom2.output.XMLOutputter;

public class MunuFile implements HolteckPropertiesFile{
	
	@Override
	public boolean create(Chip chip) {
		// create directory
		File dir = new File(chip.getPath() + "\\devices");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(chip.getPath() + "\\devices\\Holtek"); 
		if (!dir.exists())
			dir.mkdirs();
		// .menu devices
		Element devicesRoot = new Element("optionMenuItem");
		Document devicesDoc = new Document(devicesRoot);
		devicesRoot.addContent(new Element("tag").setText(chip.getChipName()));
		devicesRoot.addContent(new Element("display").setText("Holtek " + chip.getChipName()));
		devicesRoot.addContent(new Element("data").setText("$CUR_DIR$\\" + chip.getChipName() + ".i79"));
		// 输出 books.xml 文件；
		// 使xml文件 缩进效果
		Format devicesFormat = Format.getPrettyFormat();
		XMLOutputter devicesXMLOut = new XMLOutputter(devicesFormat);
		try {
			// System.out.println(chip.getPath());
			devicesXMLOut.output(devicesDoc,
					new FileOutputStream(chip.getPath() + "\\devices\\Holtek\\" + chip.getChipName() + ".menu"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return true;
	}

}
