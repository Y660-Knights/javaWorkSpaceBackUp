package com.yezi.holtek;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

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
/*		for(int i = 0; i < chipNames.size(); i ++) {
			try {
				File pro = new File(chipPropertyFilePath.get(i) + "\\chip.properties");
				FileInputStream fis = new FileInputStream(pro);
				p.load(fis);
				Chip c = new Chip(chipNames.get(i),p.getProperty("RAMSize"),p.getProperty("BlockSize"));
				c.outputIARPluginFile(chipPropertyFilePath.get(i));				
				fis.close();
				//pro.delete();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}			
		}*/
		return false;
	}
}
