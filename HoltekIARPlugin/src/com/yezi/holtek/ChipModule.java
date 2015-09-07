package com.yezi.holtek;

import java.util.ArrayList;
import java.util.List;

public class ChipModule {
	private String name = null;
	private String modulePath = null;
	private List<ChipRegister> chipRegister;
	private int baseAddress = 0;
	
	public ChipModule (String name,String modulePath,int baseAddress) {
		this.name = name;
		this.modulePath = modulePath;
		this.baseAddress = baseAddress;
		this.chipRegister = new ArrayList<ChipRegister>();
	}

	public String getModulePath() {
		return modulePath;
	}

	public String getName() {
		return name;
	}
	
	public boolean addChipRegister(ChipRegister cr) {
		return this.chipRegister.add(cr);
	}

	public int getBaseAddress() {
		return baseAddress;
	}
	
	public int getChipRegisterSize() {
		return this.chipRegister.size();
	}

	public List<ChipRegister> getChipRegisters() {
		return chipRegister;
	}
	
	
}
