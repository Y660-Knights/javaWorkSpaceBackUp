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
	private boolean release = false;
	private String path = "";
	
	private HoltekChipManager() {

	}
		
	public static HoltekChipManager getInatance() {
		if(manager == null)
			manager = new HoltekChipManager();
		
		return manager;
	}
	
	public boolean outputIARPluginFile() {
		if(this.isRelease())
			return false;
		
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

	public boolean isRelease() {
		return release;
	}

	public void setRelease(boolean release) {
		this.release = release;
	}

	public void setPath(String path) {
		this.path = path;
		fa = new FileAssistor(this.path);
		for(File f : fa.getPathDirectory()) {
			if(f.getName().equals(".svn"))
				continue;
			//chipNames.add(f.getName());
			
			//不是文件夹的统统删除
			if(this.release) {
				for(File ff : f.listFiles()) {
					if(!ff.isDirectory()) {
						ff.delete();
					}
				}
				
				//删除第二阶段的相关文件
				File td = new File(f.getPath() + "\\debugger\\Holtek\\"+f.getName()+".ddf");
				if(td.exists())
					td.delete();
				td = new File(f.getPath() + "\\debugger\\Holtek");
				if(td.exists())
					td.delete();
				td = new File(f.getPath() + "\\debugger");
				if(td.exists())
					td.delete();
				
			}else {
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
	}
}
