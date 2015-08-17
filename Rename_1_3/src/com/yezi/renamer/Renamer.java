package com.yezi.renamer;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.lang.Integer;

public class Renamer {
	//the file path
	private String path = null;
	//a list used to filled with file
	private List<File> files = null;
	
	public Renamer(String path) {
		super();
		this.path = path;
	}


	public boolean startCover() {
		//得到文件中的所有文件名
		this.files = this.getPathFile();
		if(files != null) {
			BufferedReader bufferReader = new BufferedReader(new InputStreamReader(System.in));
			int dealWay = 0;
			int length = 0;
			try {
				//选择处理点后面更新
				System.out.println("请输入处理方式:1=delete,2=insert,3=rename");
				dealWay = new Integer(bufferReader.readLine()).intValue();
			} catch (IOException e) {
				e.printStackTrace();
			}
			if(dealWay > 0) {
				switch(dealWay) {
				case 1:
					int startPosition = 0;
					System.out.println("请输入要删除的字符长度->");
					//处理这里，直接处理异常得了。如果转换出错就直接 取 字符长度
					String sLength = null;
					try {
						sLength = bufferReader.readLine();
					} catch (IOException e3) {
						e3.printStackTrace();
					}
					try {
						length = new Integer(sLength).intValue();
					} catch (NumberFormatException e2) {
						//转换出错，直接取长度
						length = sLength.length();
					}
					try {
						System.out.println("请输入删除的起始位置->\n如果是重开始删除,直接回车就可以了");
						String temp = bufferReader.readLine();
						if(temp == null || temp.equals("")) {
							dealWithDelte(length);
						}else {
							startPosition = new Integer(temp).intValue();
							dealWithDelte(length, startPosition);
						}
						
					} catch (NumberFormatException e1) {
						e1.printStackTrace();
					} catch (IOException e1) {
						e1.printStackTrace();
					}
					
					break;
				case 2:
					
					int position = 0;
					
					String insertChar = null;
					try {
						System.out.println("请输入要插入的位置");
						position = new Integer(bufferReader.readLine()).intValue();
						System.out.println("请输入要插入的字符");
						insertChar = bufferReader.readLine();
					} catch (IOException e) {
						e.printStackTrace();
					}
					
					if(position <= 0 || insertChar == null || insertChar.length() <= 0) {
						System.out.println("输入错误!");
						try {
							bufferReader.readLine();
						} catch (IOException e) {
							e.printStackTrace();
						}
						System.exit(0);
					}
					
					dealWithInsert(position, insertChar);
					break;
				case 3:
					System.out.println("你选择的顺序重命名功能。。。\n 该功能可以把指定文件重新从1000000开始命名\n解决那些长名,乱名文件\n回车后开始");
					try {
						System.in.read();
					} catch (IOException e) {
						e.printStackTrace();
					}
					dealWithRename();
					break;
				}
			}else {
				System.out.println("处理方式输入错误!");
			}
		}else {
			// files == null
			System.out.println("没有找到文件!");
		}
		return true;
	}
	
	private void dealWithInsert(int position, String insertChar) {
		StringBuffer temp = null;
		
		File covered = null;
		for(File f : files) {
			//等到了要转换成的文件名
			temp = new StringBuffer(f.getName());
			temp.insert(position, insertChar);
			
			//create file
			covered = new File(f.getParent()+ "\\" + temp.toString());	
			
			//write the file
			f.renameTo(covered);
			System.out.println(f.getName() + "..........处理成功!" + "\n");
		}
		
		showFinished();
	}


	private void dealWithDelte(int length) {
		//选择处理方式
		String temp = null;
		File covered = null;
		for(File f : files) {
			//等到了要转换成的文件名
			temp = f.getName().substring(length, f.getName().length());
			//System.out.println(temp);
			
			//create coverd file
			//String _path = f.getParent(); 得到除了文件名以前的那一部分
			covered = new File(f.getParent()+ "\\" + temp);
			
			//write the file
			f.renameTo(covered);
			System.out.println(f.getName() + "..........处理成功!" + "\n");
		}
		
		showFinished();
	}


	private void dealWithDelte(int length, int startPosition) {
		//选择处理方式
		StringBuffer temp = null;
		File covered = null;
		for(File f : files) {
			//等到了要转换成的文件名
			temp = new StringBuffer(f.getName());
			temp.delete(startPosition, startPosition + length);
			//create coverd file
			covered = new File(f.getParent()+ "\\" + temp.toString());
			
			//write the file
			f.renameTo(covered);
			System.out.println(f.getName() + "..........处理成功!" + "\n");
		}
		
		showFinished();
		

	}
	
	private List<File> getPathFile() {
		List<File> files_ = new ArrayList<File>();
		File destination = new File(this.path);
		this.tree(destination, files_);
		return files_;
	}
	
	private void tree(File file, List<File> fileList) {
		File childs[] = null;
		childs = file.listFiles();
		for (int i = 0; i < childs.length; i++) {
			if (childs[i].isDirectory()) {
				tree(childs[i], fileList);
			} else
				fileList.add(childs[i]);
		}
	}
	

	private void dealWithRename() {
		int num = 1000000;
		String temp = null;
		File covered = null;
		for(File f : files) {
			temp = f.getName();
			int last = temp.lastIndexOf(".");
			if(last == -1) {
				System.out.println("没有找到后缀名!.......\n回车程序自动退出........");
				System.exit(0);
			}
			temp = temp.substring(last, temp.length());
			
			covered = new File(f.getParent()+ "\\" + num + temp);
			//write the file
			f.renameTo(covered);
			System.out.println(f.getName() + "..........处理成功!" + "\n");
			num++;
		}
		showFinished();
	}
	
	private void showFinished() {
		System.out.println("转换完毕!");
		try {
			System.in.read();
		} catch (IOException e) {
			e.printStackTrace();
		}	
	}
	
}
