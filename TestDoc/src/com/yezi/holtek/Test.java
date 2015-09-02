package com.yezi.holtek;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.textmining.text.extraction.WordExtractor;

public class Test {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		FileInputStream in = null;
		try {
			in = new FileInputStream ("E:\\share\\IAR添加M0芯片型号\\Register Display naming\\User manual\\HT32F52230_CKCU.doc");
			//in = new FileInputStream ("E:\\share\\IAR添加M0芯片型号\\Register Display naming\\User manual\\HT32F52230_AFIO.doc");
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		WordExtractor extractor = new WordExtractor();
		String str = null;
		try {			
			str = extractor.extractText(in);
			//System.out.println(str);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		StringBuffer sb = new StringBuffer(str);
		StringBuffer sb2 = new StringBuffer("Register Descriptions");
		sb.reverse();
		sb2.reverse();
		int index = sb.indexOf(sb2.toString());
		if(index != 0) {
			sb2 = new StringBuffer(sb.substring(0,index));
			sb2.reverse();
			str = sb2.toString();
		}
			
		//获取doc中的一行
		Pattern p = Pattern.compile("\\r\\n.+");
		Matcher m = p.matcher(str);		
		while(m.find()){
			//去掉换行符等特殊符号
			String line = m.group().trim();			
	//		System.out.println(line);
			line = m.group().trim();
			Pattern p1 = Pattern.compile("(Register ?\\d? ?–? ?)\\(?(\\w+)\\)?");
			Matcher m1 = p1.matcher(line);	
			if(m1.find()) {//找寄存器的名字
				System.out.println(m1.group(2));
				while(m.find()) {//找寄存器里面每个域的名字和长度
					line = m.group().trim();		
					if(line.matches(".+Offset.+") || line.matches(".+offset.+")) {
						//LPMODReserved Type/Reset 
						Pattern p2 = Pattern.compile("[\\w+ **]+Type/Reset");
						Matcher m2 = p2.matcher(line);	
						while(m2.find()) {
							String ts = m2.group();
							ts = ts.replaceAll("Type/Reset", "");
							Pattern p3 = Pattern.compile(" *{3,9}(\\w+)");
							Matcher m3 = p3.matcher(ts);
							while(m3.find()) {
								if(m3.group(1).length() > 2)
									System.out.println(" " + m3.group(1));
							}
						}
						break;
					}	
				}
			}				
		}				
	}
}
