package com.knights.holtek;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CFileAssistor {

	private String path = "E:\\share\\progTemp\\working\\LCM_SSD1963_800x480_1.1\\LCM_SSD1963_5Inch_HT32_LCD_DVK_Demo\\showText.c";
	//private String path = "E:\\share\\progTemp\\working\\LCM_SSD1963_800x480_1.1\\LCM_SSD1963_5Inch_HT32_LCD_DVK_Demo\\QuickPlantPic.c";
	//private String path = "E:\\share\\progTemp\\working\\LCM_SSD1963_800x480_1.1\\LCM_SSD1963_5Inch_HT32_LCD_DVK_Demo\\QuickPlant.c";
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		new CFileAssistor().launch();
	}

	private void launch() {
		// TODO Auto-generated method stub
		FileAssistor fa = new FileAssistor(path);
		List<File>  files =  fa.getPathFile();
		
		for(File file : files) {
			try {				
				BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
				String line =  br.readLine();
				Pattern p;
				Matcher m;
				while(line != null){
				//	line = "void function(int i)";
					p = Pattern.compile("^\\w+ \\w+\\(.*\\)");
					m = p.matcher(line);
					if(m.find()){
						System.out.println(m.group() + ";");
					}
					line = br.readLine();
				}
				br.close();
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}		
	}
}
