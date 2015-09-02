package com.yezi.holtek;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

public class HoltekChipManager {

	private static HoltekChipManager manager = null;
	
	private List<String> chipNames = new ArrayList<String>();
	private List<File> chipPropertyFilePath = new ArrayList<File>();
	private FileAssistor fa = null;
	private Properties p = new Properties();
	
	private HoltekChipManager(String path) {
		fa = new FileAssistor(path);
		for(File f : fa.getPathDirectory()) {
			chipNames.add(f.getName());
			chipPropertyFilePath.add(f);
			//System.out.println(f.getName());
		}		
	}
		
	public static HoltekChipManager getInatance(String path) {
		if(manager == null)
			manager = new HoltekChipManager(path);
		return manager;
	}
	
	public boolean outputIARPluginFile() {
		for(int i = 0; i < chipNames.size(); i ++) {
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
		}
		return false;
	}
}
