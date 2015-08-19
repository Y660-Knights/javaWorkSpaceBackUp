package com.yezi.holtek;

import java.io.File;
import java.io.IOException;
import java.util.List;

import jxl.Cell;
import jxl.Sheet;
import jxl.Workbook;
import jxl.read.biff.BiffException;

public class ExcelFileAssistor {
	
	private String path = "E:\\share\\progTemp\\WanLiDa\\UltraTips20150818.xls";
	private String showTextFunctionName = "ZH_Hang";
	private int textPositionX = 64;
	private int textPositionY = 318;
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		new ExcelFileAssistor().launch();
	}

	private void launch() {
		// TODO Auto-generated method stub
		FileAssistor fa = new FileAssistor(path);
		List<File>  files =  fa.getPathFile();
		
		for(File file : files) {
			Workbook wb;
			try {
				wb = Workbook.getWorkbook(file);//从文件流中取得Excel工作区对象
				Sheet sheet = wb.getSheet(0);//从工作区中取得页，取得这个对象的时候既可以用名称来获得，也可以用序号。
				CFile cf = new CFile(file.getParent());
				HFile hf = new HFile(file.getParent());
				TestCFile testCF = new TestCFile(file.getParent());
				
//System.out.println(file.getParent());
		        for(int i=1; i < sheet.getRows(); i+=2){
		            for(int j=1; j < sheet.getColumns(); j++){
		            	FunctionBody fb = new FunctionBody();
				        Function f = new Function();
				        String temp = "";
				        Cell cell;
				        cell= sheet.getCell(0,1);
				        temp += cell.getContents();
				        cell= sheet.getCell(j,i);
				        temp += cell.getContents();
				        cell= sheet.getCell(j,0);
				        temp += cell.getContents();				        
				        f.setName(temp);
		                f.setReturnValue("void");
		                f.setParameters("void");
		                cell = sheet.getCell(j,i+1);
		                fb.addInvoke(showTextFunctionName + "("+textPositionX+"," + textPositionY + "," + "ZH_X0E,HangGao," + "\"" + cell.getContents()+ "\"" +");");		                
		                f.setBody(fb);
		                cf.addFunction(f);
		                hf.addFunction(f);
		                testCF.addFunction(f);
		            }		            
		        }
		        String name = "" + System.currentTimeMillis();
		        cf.addInclude(name + ".h");
		        cf.createFile(name);
		        hf.addInclude("showText.h");
		        hf.createFile(name);
		        testCF.addInclude(name + ".h");
		        testCF.setContinueFunction("while(GetKey() != KEY_DOWN_KEY2);");
		        testCF.createFile(name);
		       System.out.println(cf.getContent());
		       System.out.println(hf.getContent());
		       System.out.println(testCF.getContent());
			} catch (BiffException | IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}	        
		}		
	}

}
