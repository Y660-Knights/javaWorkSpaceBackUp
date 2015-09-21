package com.yezi.holtek;


import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
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
	
	public List<File> getPathDirectory() {
		List<File> files_ = new ArrayList<File>();
		File destination = new File(this.path);
		if(destination.isDirectory())
			for(File f : destination.listFiles()) {
				if(f.isDirectory())
					files_.add(f);
			}
		else
			return null;
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
	
	public boolean outputFile(String content) {
		File f = new File(this.path);
		if(f.isDirectory())
			return false;
		try {
			FileOutputStream fos = new FileOutputStream(f);			
			BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(fos,"UTF8"));
			bw.write(content);
			bw.flush();
			bw.close();
			fos.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return true;
	}

	public void copyPath2Path(String src, String dest) {
		// TODO Auto-generated method stub
		File fsrc = new File(src);
		
		for(File f : fsrc.listFiles()) {
			if(f != null && f.exists() && !f.isDirectory()) {
				File ft = new File(dest + "\\" + f.getName());
				if(ft.exists())
					ft.delete();				
				f.renameTo(ft);	
			}
		}
	}
	
}
