package com.yezi.holtek;

import java.util.ArrayList;
import java.util.List;

public class HFile {

	private FileAssistor fa;
	private String path;
	private List<Function> functions;
	private String include = "";
	
	public HFile (String path) {
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
			content += f.getPrototype() + ";\r\n";
		}
		return content;
	}

	public void createFile(String name) {
		if(this.fa == null)
			this.fa = new FileAssistor(path + "\\" + name + ".h");
		
		fa.outputFile("#ifndef\t__" + name.toUpperCase() + "_H\r\n"+ "#define\t__" + name.toUpperCase() + "_H\r\n\r\n" +this.getContent() + "\r\n#endif //__"+name.toUpperCase()+"_H"+"\r\n");
	}
	
	public void addInclude(String include) {
		this.include += "#include\t\"" + include + "\"\r\n\r\n";
	}

}
