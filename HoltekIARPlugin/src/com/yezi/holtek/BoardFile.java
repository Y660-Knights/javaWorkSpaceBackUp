package com.yezi.holtek;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import org.jdom2.Document;
import org.jdom2.Element;
import org.jdom2.output.Format;
import org.jdom2.output.XMLOutputter;

public class BoardFile implements HolteckPropertiesFile{

	@Override
	public boolean create(Chip chip) {
		// TODO Auto-generated method stub
		// .board
		File dir = new File(chip.getPath() + "\\flashloader");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(chip.getPath() + "\\flashloader\\Holtek");
		if (!dir.exists())
			dir.mkdirs();
		
		// .board flashloader\Holtek
		Element boardRoot = new Element("flash_board");
		Document boardDoc = new Document(boardRoot);
		Element boardpass = new Element("pass");
		Element boardpass2 = new Element("pass");

		boardpass.addContent(new Element("loader")
				.setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\Flash" + chip.getChipName() + ".flash"));
		boardpass.addContent(new Element("range").setText("CODE 0x00000000 " + chip.getCodeRange()));

		boardpass2.addContent(new Element("loader")
				.setText("$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\Flash" + chip.getChipName() + "_op.flash"));
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
					new FileOutputStream(chip.getPath() + "\\flashloader\\Holtek\\Flash" + chip.getChipName() + ".board"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return true;
	}

}
