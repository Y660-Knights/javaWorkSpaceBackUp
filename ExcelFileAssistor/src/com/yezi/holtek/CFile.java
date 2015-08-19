package com.yezi.holtek;

import java.util.ArrayList;
import java.util.List;

public class CFile {
	private FileAssistor fa;
	private String path;
	private List<Function> functions;
	private String include = "";
	
	public CFile (String path) {
		if(this.functions == null) {
			this.functions = new ArrayList<Function>();
		}
		this.path = path;		
	}
	
	public boolean addFunction(Function f) {
		return this.functions.add(f);
	}
	
	public String getContent() {
		String content = "";
		content += this.include;
		for(Function f : this.functions) {
			content += f.getFunction();
		}
		return content;
	}

	public void createFile(String name) {
		if(this.fa == null)
			this.fa = new FileAssistor(path + "\\" + name + ".c");
		fa.outputFile(this.getContent());
	}

	public void addInclude(String include) {
		this.include += "#include\t\"" + include + "\"\r\n\r\n";
	}
	
}
