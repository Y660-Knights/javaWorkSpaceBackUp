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
		
		int matchCount = 0;		
		Pattern p1 = Pattern.compile("Register ?\\d? ?\\((\\w+)\\)");
		
		while(m.find()) {
			String line = m.group().trim();			
			Matcher m1 = p1.matcher(line);	
			if(m1.find()) {
				matchCount ++;
			}			
		}
		
		if(matchCount > 2) { //最少有两个寄存器,说明该表达式正确
			p1 = Pattern.compile("Register ?\\d? ?\\((\\w+)\\)");
			System.out.println("Reg count = " + matchCount); 
		}else { //更换表达式
			p1 = Pattern.compile("Register ?\\d? ?– ?\\(?(\\w+)\\)?");
		}
		
		m = p.matcher(str);
		while(m.find()) {
			//去掉换行符等特殊符号
			String line = m.group().trim();			
	//		System.out.println(line);
			line = m.group().trim();
			//这里的表达式分两种情况，有()  和 - 
			//Pattern p1 = Pattern.compile("(Register ?\\d? ?–? ?)\\(?(\\w+)\\)?");
			Matcher m1 = p1.matcher(line);
			if(m1.find()) {//找寄存器的名字
				matchCount ++;
				System.out.println(m1.group(1));
				while(m.find()) {//获取全文的下一行
					line = m.group().trim();		
					if(line.matches(".+Offset.+") || line.matches(".+offset.+")) { //如果本行含有 offset 说明是寄存器域名所在行
						//LPMODReserved Type/Reset 
						Pattern p2 = Pattern.compile("[\\w+ **]+Type/Reset");
						Matcher m2 = p2.matcher(line);	
						while(m2.find()) { //以Type/Reset为标识，把寄存器域名所在行分成一个一个的小单位，便于处理
							String ts = m2.group();
							ts = ts.replaceAll("Type/Reset", "");
							Pattern p3 = Pattern.compile("( *{1,9})(\\w+)");
							Matcher m3 = p3.matcher(ts);
							while(m3.find()) { //这里获得了域名,但是域名的长度无法获取。
								if(m3.group(2).length() > 2 &&  !m3.group(2).startsWith("0"))
									if(m3.group(2).length() < "Reserve".length())
										System.out.println(" " + m3.group(2) + "\t\t:\t" + m3.group(1).length());
									else
										System.out.println(" " + m3.group(2) + "\t:\t" + m3.group(1).length());
							}
						}
						break;
					}	
				}
			}
		}
	}
}
