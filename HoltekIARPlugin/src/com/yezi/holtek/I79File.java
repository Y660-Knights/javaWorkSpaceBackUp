package com.yezi.holtek;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import org.ini4j.Wini;

public class I79File implements HolteckPropertiesFile{

	@Override
	public boolean create(Chip chip) {
		//检查目录是否存在
		File dir = new File(chip.getPath()+ "\\devices");
		if (!dir.exists())
			dir.mkdirs();
		dir = new File(chip.getPath() + "\\devices\\Holtek");
		if (!dir.exists())
			dir.mkdirs();
		
		// .i79
		String i79FileName = chip.getPath() + "\\devices\\Holtek\\" + chip.getChipName() + ".i79";
		try {
			File fIni = new File(i79FileName);
			OutputStream os = new FileOutputStream(fIni);
			os.close();
			Wini ini = new Wini(fIni);
			ini.put("FILEFORMAT", "rev", 1.6);

			ini.put("CHIP", "name", chip.getChipName());
			ini.put("CHIP", "endiansupport", "le");
			ini.put("CHIP", "thumbsupport", "true");
			ini.put("CHIP", "armsupport", "false");
			ini.put("CHIP", "fpu", "None");

			ini.put("CORE", "name", "Cortex-M0");

			ini.put("DDF FILE", "name", "Holtek\\" + chip.getChipName() + ".ddf");

			ini.put("LINKER FILE", "name",
					"$TOOLKIT_DIR$\\config\\linker\\Holtek\\" + chip.getChipName().toLowerCase() + ".icf");

			ini.put("FLASH LOADER", "little",
					"$TOOLKIT_DIR$\\config\\flashloader\\Holtek\\Flash" + chip.getChipName() + ".board");

			ini.store();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return true;
	}
	
}
