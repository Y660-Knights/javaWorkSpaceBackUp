package com.yezi.holtek;

import java.io.File;

public class Holtek {
	static String path = "E:\\share\\IAR添加M0芯片型号\\Holteck";

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		HoltekChipManager hcm = HoltekChipManager.getInatance();
		// hcm.setRelease(true);
		hcm.setPath(path);
		System.out.println("create properties file complete ? \r\n\t" + hcm.outputIARPluginFile());

		// 把所有文件都拷贝到一个公共目录中,其实不用删除的。
		String outputPathName = "output";
		String outputPath = new File(path).getParent();

		String outputPath1 = outputPath + "\\" + outputPathName + "\\devices";
		String outputPath2 = outputPath + "\\" + outputPathName + "\\flashloader";
		String outputPath3 = outputPath + "\\" + outputPathName + "\\inc";
		String outputPath4 = outputPath + "\\" + outputPathName + "\\debugger";

		File temp = new File(outputPath + "\\" + outputPathName);
		if (!temp.exists()) {
			temp.mkdirs();
		}

		temp = new File(outputPath1);
		if (!temp.exists()) {
			temp.mkdirs();
		}
		temp = new File(outputPath2);
		if (!temp.exists()) {
			temp.mkdirs();
		}
		temp = new File(outputPath3);
		if (!temp.exists()) {
			temp.mkdirs();
		}
		
		temp = new File(outputPath4);
		if (!temp.exists()) {
			temp.mkdirs();
		}
		
		FileAssistor fa = new FileAssistor(path);
		for (File f : fa.getPathDirectory()) {
			if (!f.getName().equals(outputPathName) && !f.getName().equals(".svn")) {
				String src = f.getPath() + "\\devices\\Holtek";
				String dest = outputPath1;
				fa.copyPath2Path(src, dest);

				src = f.getPath() + "\\flashloader\\Holtek";
				dest = outputPath2;
				fa.copyPath2Path(src, dest);

				src = f.getPath() + "\\inc\\Holtek";
				dest = outputPath3;
				fa.copyPath2Path(src, dest);
				
				src = f.getPath() + "\\debugger\\Holtek";
				dest = outputPath4;
				fa.copyPath2Path(src, dest);
			}
		}
	}
}
