package com.yezi.holtek;

public class Holtek {
	static String path = "E:\\share\\IAR添加M0芯片型号\\Holteck";
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("create properties file complete ? \r\n\t" + HoltekChipManager.getInatance(path).outputIARPluginFile());
	}
}
