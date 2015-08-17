package com.yezi.renamer;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Lanucher {
	public static void main(String[] args) {
		//输入路径
		BufferedReader bufferReader = new BufferedReader(new InputStreamReader(System.in));
		String path = null;
		try {
			System.out.println("请输入要转换的路径->");
			path = bufferReader.readLine();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		if(path != null && path.length() > 0) {
			new Lanucher().start(path);
		}else {
			System.out.println("输入错误！ 请重启本程序,任意键结束");
			try {
				bufferReader.read();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	private void start(String path) {
		Renamer rename = new Renamer(path);
		rename.startCover();
	}
	
}
