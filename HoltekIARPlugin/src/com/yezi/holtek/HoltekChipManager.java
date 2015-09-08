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

public class HoltekChipManager {

	private static HoltekChipManager manager = null;
	private List<Chip> chips = new ArrayList<Chip>();
	private FileAssistor fa = null;
	private Properties p = new Properties();
	
	private HoltekChipManager(String path) {
		fa = new FileAssistor(path);
		for(File f : fa.getPathDirectory()) {
			if(f.getName().equals(".svn"))
				continue;
			//chipNames.add(f.getName());
			File pro = new File(f.getPath() + "\\chip.properties");
			FileInputStream fis = null;
			try {
				fis = new FileInputStream(pro);
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			try {
				p.load(fis);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Chip c = new Chip(f.getPath(),f.getName(),p.getProperty("RAMSize"),p.getProperty("BlockSize"));
			this.chips.add(c);
		}		
	}
		
	public static HoltekChipManager getInatance(String path) {
		if(manager == null)
			manager = new HoltekChipManager(path);
		return manager;
	}
	
	public boolean outputIARPluginFile() {
		List<HolteckPropertiesFile> hpfs = new ArrayList<>();
		hpfs.add(new I79File());
		hpfs.add(new MenuFile());
		hpfs.add(new BoardFile());
		hpfs.add(new FlashFile());
		hpfs.add(new OpflashFile());
		hpfs.add(new HFile());
		hpfs.add(new DdfFile());
		for(Chip c : chips) {
			for(HolteckPropertiesFile hpf : hpfs) {
				hpf.create(c);
			}
		}
				
		return true;
	}
}
