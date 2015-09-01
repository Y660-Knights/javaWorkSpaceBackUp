package com.yezi.holtek;

public class Holtek {
	static String path = "E:\\Holteck";
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		HoltekChipManager.getInatance(path).outputIARPluginFile();
	}

}
