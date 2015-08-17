package com.knights.holtek;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class FileAssistor {
	private String path;
	
	public FileAssistor(String path) {
		this.path = path;
	}	

	public List<File> getPathFile() {
		List<File> files_ = new ArrayList<File>();
		File destination = new File(this.path);
		if(destination.isDirectory())
			this.tree(destination, files_);
		else
			files_.add(destination);
		return files_;
	}
	
	private void tree(File file, List<File> fileList) {
		File childs[] = null;
		childs = file.listFiles();
		for (int i = 0; i < childs.length; i++) {
			if (childs[i].isDirectory()) {
				tree(childs[i], fileList);
			} else
				fileList.add(childs[i]);
		}
	}			
}
