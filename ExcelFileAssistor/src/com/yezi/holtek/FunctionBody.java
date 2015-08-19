package com.yezi.holtek;

import java.util.ArrayList;
import java.util.List;

public class FunctionBody {
	private List<String> invokes;
	
	public FunctionBody () {
		if(this.invokes == null)
			this.invokes = new ArrayList<String>();
	}
	
	public List<String> getInvoke() {
		// TODO Auto-generated method stub		
		return this.invokes;
	}
	
	public boolean addInvoke(String i) {
		return this.invokes.add(i);
	}
}
