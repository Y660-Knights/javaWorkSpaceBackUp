package com.yezi.holtek;

import java.util.ArrayList;
import java.util.List;

public class HFile {

	private FileAssistor fa;
	private String path;
	private List<ChipRegister> ChipRegister;
	private String include = "";
	private String ifdef = "";
	private String call = "";
	
	public HFile (String path) {
		if(this.ChipRegister == null) {
			this.ChipRegister = new ArrayList<ChipRegister>();
		}
		this.path = path;		
	}
	
	public boolean addChipRegister(ChipRegister f) {
		return this.ChipRegister.add(f);
	}
	
	public String getContent() {
		String content = "";
		content += this.include;
		content += this.ifdef;
		for(ChipRegister f : this.ChipRegister) {
			content += f.getTypedefText();
		}
		content += "#endif\r\n\r\n";
		content += call;
		
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

	public void addIfdef(String def) {
		// TODO Auto-generated method stub
		this.ifdef += "\r\n" + def + "\r\n\r\n";
	}

	public void addCall(String string, String string2) {
		// TODO Auto-generated method stub
		this.call += string + "("+string2+");\r\n";
	}

}
