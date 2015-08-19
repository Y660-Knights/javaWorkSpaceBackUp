package com.yezi.holtek;

public class Function {
	
	private String returnValue;
	private String name;
	private StringBuffer parameters;
	
	private FunctionBody body;
	
	public Function() {
		if(this.parameters == null)
			this.parameters = new StringBuffer();
	}
	
	public String getPrototype() {
		return this.returnValue + " " + this.name + "(" + this.parameters.toString() + ")"; 
	}
	
	public String getFunction(){
		StringBuffer sb = new StringBuffer();
		sb.append(this.getPrototype());
		sb.append("\r\n");
		sb.append("{");
					
		for(String i : this.body.getInvoke()) {
			sb.append("\r\n");//换行符
			sb.append("\t");//tab
			sb.append(i);
			sb.append("\r\n");//换行符
		}						
		
		sb.append("}");
		sb.append("\r\n");//tab
		return sb.toString();
	}

	public void setReturnValue(String returnValue) {
		this.returnValue = returnValue;
	}

	public void setName(String name) {
		this.name = name;
	}

	public void setParameters(String parameter) {		
		
		if(this.parameters.toString().equals(""))
			this.parameters.append(parameter);
		else
			this.parameters.append("," + parameter);
	}

	public void setBody(FunctionBody body) {
		this.body = body;
	}
	
	public String getFunctionCall(){
		return this.name + "();";
	}
	
}
