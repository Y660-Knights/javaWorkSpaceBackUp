package com.yezi.holtek;

import java.util.ArrayList;
import java.util.List;

public class TestCFile {
	private FileAssistor fa;
	private String path;
	private List<Function> functions;
	private String continueFunction = "";
	private String include = "";
	
	public TestCFile (String path) {
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
		content += "\r\n";
		for(Function f : this.functions) {			
			content += "\t" + f.getFunctionCall() + "\r\n";
			content += continueFunction;
		}
		return content;
	}

	public void createFile(String name) {
		String content = "";
		content += this.include;
		if(this.fa == null)
			this.fa = new FileAssistor(path + "\\Test" + name + ".c");
		content += "void startTest(void)\r\n{\r\n";		
		content +=this.getContent()+"\r\n";
		content += "\r\n}\r\n";
		fa.outputFile(content);
	}

	public void addContinueFunction(String continueFunction) {
		this.continueFunction += "\t" + continueFunction+ "\r\n";
	}
	
	public void addInclude(String include) {
		this.include += "#include\t\"" + include + "\"\r\n\r\n";
	}
}
