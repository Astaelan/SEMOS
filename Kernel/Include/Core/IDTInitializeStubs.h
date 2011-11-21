#pragma once

	// Reserved by CPU
	InitializeGate(0x00, reinterpret_cast<uint32_t>(IDTISR00));
	InitializeGate(0x01, reinterpret_cast<uint32_t>(IDTISR01));
	InitializeGate(0x02, reinterpret_cast<uint32_t>(IDTISR02));
	InitializeGate(0x03, reinterpret_cast<uint32_t>(IDTISR03));
	InitializeGate(0x04, reinterpret_cast<uint32_t>(IDTISR04));
	InitializeGate(0x05, reinterpret_cast<uint32_t>(IDTISR05));
	InitializeGate(0x06, reinterpret_cast<uint32_t>(IDTISR06));
	InitializeGate(0x07, reinterpret_cast<uint32_t>(IDTISR07));
	InitializeGate(0x08, reinterpret_cast<uint32_t>(IDTISR08));
	InitializeGate(0x09, reinterpret_cast<uint32_t>(IDTISR09));
	InitializeGate(0x0A, reinterpret_cast<uint32_t>(IDTISR0A));
	InitializeGate(0x0B, reinterpret_cast<uint32_t>(IDTISR0B));
	InitializeGate(0x0C, reinterpret_cast<uint32_t>(IDTISR0C));
	InitializeGate(0x0D, reinterpret_cast<uint32_t>(IDTISR0D));
	InitializeGate(0x0E, reinterpret_cast<uint32_t>(IDTISR0E));
	InitializeGate(0x0F, reinterpret_cast<uint32_t>(IDTISR0F));
	InitializeGate(0x10, reinterpret_cast<uint32_t>(IDTISR10));
	InitializeGate(0x11, reinterpret_cast<uint32_t>(IDTISR11));
	InitializeGate(0x12, reinterpret_cast<uint32_t>(IDTISR12));
	InitializeGate(0x13, reinterpret_cast<uint32_t>(IDTISR13));
	InitializeGate(0x14, reinterpret_cast<uint32_t>(IDTISR14));
	InitializeGate(0x15, reinterpret_cast<uint32_t>(IDTISR15));
	InitializeGate(0x16, reinterpret_cast<uint32_t>(IDTISR16));
	InitializeGate(0x17, reinterpret_cast<uint32_t>(IDTISR17));
	InitializeGate(0x18, reinterpret_cast<uint32_t>(IDTISR18));
	InitializeGate(0x19, reinterpret_cast<uint32_t>(IDTISR19));
	InitializeGate(0x1A, reinterpret_cast<uint32_t>(IDTISR1A));
	InitializeGate(0x1B, reinterpret_cast<uint32_t>(IDTISR1B));
	InitializeGate(0x1C, reinterpret_cast<uint32_t>(IDTISR1C));
	InitializeGate(0x1D, reinterpret_cast<uint32_t>(IDTISR1D));
	InitializeGate(0x1E, reinterpret_cast<uint32_t>(IDTISR1E));
	InitializeGate(0x1F, reinterpret_cast<uint32_t>(IDTISR1F));
	// Reserved for IRQ
	InitializeGate(0x20, reinterpret_cast<uint32_t>(IDTIRQ00));
	InitializeGate(0x21, reinterpret_cast<uint32_t>(IDTIRQ01));
	InitializeGate(0x22, reinterpret_cast<uint32_t>(IDTIRQ02));
	InitializeGate(0x23, reinterpret_cast<uint32_t>(IDTIRQ03));
	InitializeGate(0x24, reinterpret_cast<uint32_t>(IDTIRQ04));
	InitializeGate(0x25, reinterpret_cast<uint32_t>(IDTIRQ05));
	InitializeGate(0x26, reinterpret_cast<uint32_t>(IDTIRQ06));
	InitializeGate(0x27, reinterpret_cast<uint32_t>(IDTIRQ07));
	InitializeGate(0x28, reinterpret_cast<uint32_t>(IDTIRQ08));
	InitializeGate(0x29, reinterpret_cast<uint32_t>(IDTIRQ09));
	InitializeGate(0x2A, reinterpret_cast<uint32_t>(IDTIRQ0A));
	InitializeGate(0x2B, reinterpret_cast<uint32_t>(IDTIRQ0B));
	InitializeGate(0x2C, reinterpret_cast<uint32_t>(IDTIRQ0C));
	InitializeGate(0x2D, reinterpret_cast<uint32_t>(IDTIRQ0D));
	InitializeGate(0x2E, reinterpret_cast<uint32_t>(IDTIRQ0E));
	InitializeGate(0x2F, reinterpret_cast<uint32_t>(IDTIRQ0F));
	// Usable
	InitializeGate(0x30, reinterpret_cast<uint32_t>(IDTISR30));
	InitializeGate(0x31, reinterpret_cast<uint32_t>(IDTISR31));
	InitializeGate(0x32, reinterpret_cast<uint32_t>(IDTISR32));
	InitializeGate(0x33, reinterpret_cast<uint32_t>(IDTISR33));
	InitializeGate(0x34, reinterpret_cast<uint32_t>(IDTISR34));
	InitializeGate(0x35, reinterpret_cast<uint32_t>(IDTISR35));
	InitializeGate(0x36, reinterpret_cast<uint32_t>(IDTISR36));
	InitializeGate(0x37, reinterpret_cast<uint32_t>(IDTISR37));
	InitializeGate(0x38, reinterpret_cast<uint32_t>(IDTISR38));
	InitializeGate(0x39, reinterpret_cast<uint32_t>(IDTISR39));
	InitializeGate(0x3A, reinterpret_cast<uint32_t>(IDTISR3A));
	InitializeGate(0x3B, reinterpret_cast<uint32_t>(IDTISR3B));
	InitializeGate(0x3C, reinterpret_cast<uint32_t>(IDTISR3C));
	InitializeGate(0x3D, reinterpret_cast<uint32_t>(IDTISR3D));
	InitializeGate(0x3E, reinterpret_cast<uint32_t>(IDTISR3E));
	InitializeGate(0x3F, reinterpret_cast<uint32_t>(IDTISR3F));
	InitializeGate(0x40, reinterpret_cast<uint32_t>(IDTISR40));
	InitializeGate(0x41, reinterpret_cast<uint32_t>(IDTISR41));
	InitializeGate(0x42, reinterpret_cast<uint32_t>(IDTISR42));
	InitializeGate(0x43, reinterpret_cast<uint32_t>(IDTISR43));
	InitializeGate(0x44, reinterpret_cast<uint32_t>(IDTISR44));
	InitializeGate(0x45, reinterpret_cast<uint32_t>(IDTISR45));
	InitializeGate(0x46, reinterpret_cast<uint32_t>(IDTISR46));
	InitializeGate(0x47, reinterpret_cast<uint32_t>(IDTISR47));
	InitializeGate(0x48, reinterpret_cast<uint32_t>(IDTISR48));
	InitializeGate(0x49, reinterpret_cast<uint32_t>(IDTISR49));
	InitializeGate(0x4A, reinterpret_cast<uint32_t>(IDTISR4A));
	InitializeGate(0x4B, reinterpret_cast<uint32_t>(IDTISR4B));
	InitializeGate(0x4C, reinterpret_cast<uint32_t>(IDTISR4C));
	InitializeGate(0x4D, reinterpret_cast<uint32_t>(IDTISR4D));
	InitializeGate(0x4E, reinterpret_cast<uint32_t>(IDTISR4E));
	InitializeGate(0x4F, reinterpret_cast<uint32_t>(IDTISR4F));
	InitializeGate(0x50, reinterpret_cast<uint32_t>(IDTISR50));
	InitializeGate(0x51, reinterpret_cast<uint32_t>(IDTISR51));
	InitializeGate(0x52, reinterpret_cast<uint32_t>(IDTISR52));
	InitializeGate(0x53, reinterpret_cast<uint32_t>(IDTISR53));
	InitializeGate(0x54, reinterpret_cast<uint32_t>(IDTISR54));
	InitializeGate(0x55, reinterpret_cast<uint32_t>(IDTISR55));
	InitializeGate(0x56, reinterpret_cast<uint32_t>(IDTISR56));
	InitializeGate(0x57, reinterpret_cast<uint32_t>(IDTISR57));
	InitializeGate(0x58, reinterpret_cast<uint32_t>(IDTISR58));
	InitializeGate(0x59, reinterpret_cast<uint32_t>(IDTISR59));
	InitializeGate(0x5A, reinterpret_cast<uint32_t>(IDTISR5A));
	InitializeGate(0x5B, reinterpret_cast<uint32_t>(IDTISR5B));
	InitializeGate(0x5C, reinterpret_cast<uint32_t>(IDTISR5C));
	InitializeGate(0x5D, reinterpret_cast<uint32_t>(IDTISR5D));
	InitializeGate(0x5E, reinterpret_cast<uint32_t>(IDTISR5E));
	InitializeGate(0x5F, reinterpret_cast<uint32_t>(IDTISR5F));
	InitializeGate(0x60, reinterpret_cast<uint32_t>(IDTISR60));
	InitializeGate(0x61, reinterpret_cast<uint32_t>(IDTISR61));
	InitializeGate(0x62, reinterpret_cast<uint32_t>(IDTISR62));
	InitializeGate(0x63, reinterpret_cast<uint32_t>(IDTISR63));
	InitializeGate(0x64, reinterpret_cast<uint32_t>(IDTISR64));
	InitializeGate(0x65, reinterpret_cast<uint32_t>(IDTISR65));
	InitializeGate(0x66, reinterpret_cast<uint32_t>(IDTISR66));
	InitializeGate(0x67, reinterpret_cast<uint32_t>(IDTISR67));
	InitializeGate(0x68, reinterpret_cast<uint32_t>(IDTISR68));
	InitializeGate(0x69, reinterpret_cast<uint32_t>(IDTISR69));
	InitializeGate(0x6A, reinterpret_cast<uint32_t>(IDTISR6A));
	InitializeGate(0x6B, reinterpret_cast<uint32_t>(IDTISR6B));
	InitializeGate(0x6C, reinterpret_cast<uint32_t>(IDTISR6C));
	InitializeGate(0x6D, reinterpret_cast<uint32_t>(IDTISR6D));
	InitializeGate(0x6E, reinterpret_cast<uint32_t>(IDTISR6E));
	InitializeGate(0x6F, reinterpret_cast<uint32_t>(IDTISR6F));
	InitializeGate(0x70, reinterpret_cast<uint32_t>(IDTISR70));
	InitializeGate(0x71, reinterpret_cast<uint32_t>(IDTISR71));
	InitializeGate(0x72, reinterpret_cast<uint32_t>(IDTISR72));
	InitializeGate(0x73, reinterpret_cast<uint32_t>(IDTISR73));
	InitializeGate(0x74, reinterpret_cast<uint32_t>(IDTISR74));
	InitializeGate(0x75, reinterpret_cast<uint32_t>(IDTISR75));
	InitializeGate(0x76, reinterpret_cast<uint32_t>(IDTISR76));
	InitializeGate(0x77, reinterpret_cast<uint32_t>(IDTISR77));
	InitializeGate(0x78, reinterpret_cast<uint32_t>(IDTISR78));
	InitializeGate(0x79, reinterpret_cast<uint32_t>(IDTISR79));
	InitializeGate(0x7A, reinterpret_cast<uint32_t>(IDTISR7A));
	InitializeGate(0x7B, reinterpret_cast<uint32_t>(IDTISR7B));
	InitializeGate(0x7C, reinterpret_cast<uint32_t>(IDTISR7C));
	InitializeGate(0x7D, reinterpret_cast<uint32_t>(IDTISR7D));
	InitializeGate(0x7E, reinterpret_cast<uint32_t>(IDTISR7E));
	InitializeGate(0x7F, reinterpret_cast<uint32_t>(IDTISR7F));
	InitializeGate(0x80, reinterpret_cast<uint32_t>(IDTISR80));
	InitializeGate(0x81, reinterpret_cast<uint32_t>(IDTISR81));
	InitializeGate(0x82, reinterpret_cast<uint32_t>(IDTISR82));
	InitializeGate(0x83, reinterpret_cast<uint32_t>(IDTISR83));
	InitializeGate(0x84, reinterpret_cast<uint32_t>(IDTISR84));
	InitializeGate(0x85, reinterpret_cast<uint32_t>(IDTISR85));
	InitializeGate(0x86, reinterpret_cast<uint32_t>(IDTISR86));
	InitializeGate(0x87, reinterpret_cast<uint32_t>(IDTISR87));
	InitializeGate(0x88, reinterpret_cast<uint32_t>(IDTISR88));
	InitializeGate(0x89, reinterpret_cast<uint32_t>(IDTISR89));
	InitializeGate(0x8A, reinterpret_cast<uint32_t>(IDTISR8A));
	InitializeGate(0x8B, reinterpret_cast<uint32_t>(IDTISR8B));
	InitializeGate(0x8C, reinterpret_cast<uint32_t>(IDTISR8C));
	InitializeGate(0x8D, reinterpret_cast<uint32_t>(IDTISR8D));
	InitializeGate(0x8E, reinterpret_cast<uint32_t>(IDTISR8E));
	InitializeGate(0x8F, reinterpret_cast<uint32_t>(IDTISR8F));
	InitializeGate(0x90, reinterpret_cast<uint32_t>(IDTISR90));
	InitializeGate(0x91, reinterpret_cast<uint32_t>(IDTISR91));
	InitializeGate(0x92, reinterpret_cast<uint32_t>(IDTISR92));
	InitializeGate(0x93, reinterpret_cast<uint32_t>(IDTISR93));
	InitializeGate(0x94, reinterpret_cast<uint32_t>(IDTISR94));
	InitializeGate(0x95, reinterpret_cast<uint32_t>(IDTISR95));
	InitializeGate(0x96, reinterpret_cast<uint32_t>(IDTISR96));
	InitializeGate(0x97, reinterpret_cast<uint32_t>(IDTISR97));
	InitializeGate(0x98, reinterpret_cast<uint32_t>(IDTISR98));
	InitializeGate(0x99, reinterpret_cast<uint32_t>(IDTISR99));
	InitializeGate(0x9A, reinterpret_cast<uint32_t>(IDTISR9A));
	InitializeGate(0x9B, reinterpret_cast<uint32_t>(IDTISR9B));
	InitializeGate(0x9C, reinterpret_cast<uint32_t>(IDTISR9C));
	InitializeGate(0x9D, reinterpret_cast<uint32_t>(IDTISR9D));
	InitializeGate(0x9E, reinterpret_cast<uint32_t>(IDTISR9E));
	InitializeGate(0x9F, reinterpret_cast<uint32_t>(IDTISR9F));
	InitializeGate(0xA0, reinterpret_cast<uint32_t>(IDTISRA0));
	InitializeGate(0xA1, reinterpret_cast<uint32_t>(IDTISRA1));
	InitializeGate(0xA2, reinterpret_cast<uint32_t>(IDTISRA2));
	InitializeGate(0xA3, reinterpret_cast<uint32_t>(IDTISRA3));
	InitializeGate(0xA4, reinterpret_cast<uint32_t>(IDTISRA4));
	InitializeGate(0xA5, reinterpret_cast<uint32_t>(IDTISRA5));
	InitializeGate(0xA6, reinterpret_cast<uint32_t>(IDTISRA6));
	InitializeGate(0xA7, reinterpret_cast<uint32_t>(IDTISRA7));
	InitializeGate(0xA8, reinterpret_cast<uint32_t>(IDTISRA8));
	InitializeGate(0xA9, reinterpret_cast<uint32_t>(IDTISRA9));
	InitializeGate(0xAA, reinterpret_cast<uint32_t>(IDTISRAA));
	InitializeGate(0xAB, reinterpret_cast<uint32_t>(IDTISRAB));
	InitializeGate(0xAC, reinterpret_cast<uint32_t>(IDTISRAC));
	InitializeGate(0xAD, reinterpret_cast<uint32_t>(IDTISRAD));
	InitializeGate(0xAE, reinterpret_cast<uint32_t>(IDTISRAE));
	InitializeGate(0xAF, reinterpret_cast<uint32_t>(IDTISRAF));
	InitializeGate(0xB0, reinterpret_cast<uint32_t>(IDTISRB0));
	InitializeGate(0xB1, reinterpret_cast<uint32_t>(IDTISRB1));
	InitializeGate(0xB2, reinterpret_cast<uint32_t>(IDTISRB2));
	InitializeGate(0xB3, reinterpret_cast<uint32_t>(IDTISRB3));
	InitializeGate(0xB4, reinterpret_cast<uint32_t>(IDTISRB4));
	InitializeGate(0xB5, reinterpret_cast<uint32_t>(IDTISRB5));
	InitializeGate(0xB6, reinterpret_cast<uint32_t>(IDTISRB6));
	InitializeGate(0xB7, reinterpret_cast<uint32_t>(IDTISRB7));
	InitializeGate(0xB8, reinterpret_cast<uint32_t>(IDTISRB8));
	InitializeGate(0xB9, reinterpret_cast<uint32_t>(IDTISRB9));
	InitializeGate(0xBA, reinterpret_cast<uint32_t>(IDTISRBA));
	InitializeGate(0xBB, reinterpret_cast<uint32_t>(IDTISRBB));
	InitializeGate(0xBC, reinterpret_cast<uint32_t>(IDTISRBC));
	InitializeGate(0xBD, reinterpret_cast<uint32_t>(IDTISRBD));
	InitializeGate(0xBE, reinterpret_cast<uint32_t>(IDTISRBE));
	InitializeGate(0xBF, reinterpret_cast<uint32_t>(IDTISRBF));
	InitializeGate(0xC0, reinterpret_cast<uint32_t>(IDTISRC0));
	InitializeGate(0xC1, reinterpret_cast<uint32_t>(IDTISRC1));
	InitializeGate(0xC2, reinterpret_cast<uint32_t>(IDTISRC2));
	InitializeGate(0xC3, reinterpret_cast<uint32_t>(IDTISRC3));
	InitializeGate(0xC4, reinterpret_cast<uint32_t>(IDTISRC4));
	InitializeGate(0xC5, reinterpret_cast<uint32_t>(IDTISRC5));
	InitializeGate(0xC6, reinterpret_cast<uint32_t>(IDTISRC6));
	InitializeGate(0xC7, reinterpret_cast<uint32_t>(IDTISRC7));
	InitializeGate(0xC8, reinterpret_cast<uint32_t>(IDTISRC8));
	InitializeGate(0xC9, reinterpret_cast<uint32_t>(IDTISRC9));
	InitializeGate(0xCA, reinterpret_cast<uint32_t>(IDTISRCA));
	InitializeGate(0xCB, reinterpret_cast<uint32_t>(IDTISRCB));
	InitializeGate(0xCC, reinterpret_cast<uint32_t>(IDTISRCC));
	InitializeGate(0xCD, reinterpret_cast<uint32_t>(IDTISRCD));
	InitializeGate(0xCE, reinterpret_cast<uint32_t>(IDTISRCE));
	InitializeGate(0xCF, reinterpret_cast<uint32_t>(IDTISRCF));
	InitializeGate(0xD0, reinterpret_cast<uint32_t>(IDTISRD0));
	InitializeGate(0xD1, reinterpret_cast<uint32_t>(IDTISRD1));
	InitializeGate(0xD2, reinterpret_cast<uint32_t>(IDTISRD2));
	InitializeGate(0xD3, reinterpret_cast<uint32_t>(IDTISRD3));
	InitializeGate(0xD4, reinterpret_cast<uint32_t>(IDTISRD4));
	InitializeGate(0xD5, reinterpret_cast<uint32_t>(IDTISRD5));
	InitializeGate(0xD6, reinterpret_cast<uint32_t>(IDTISRD6));
	InitializeGate(0xD7, reinterpret_cast<uint32_t>(IDTISRD7));
	InitializeGate(0xD8, reinterpret_cast<uint32_t>(IDTISRD8));
	InitializeGate(0xD9, reinterpret_cast<uint32_t>(IDTISRD9));
	InitializeGate(0xDA, reinterpret_cast<uint32_t>(IDTISRDA));
	InitializeGate(0xDB, reinterpret_cast<uint32_t>(IDTISRDB));
	InitializeGate(0xDC, reinterpret_cast<uint32_t>(IDTISRDC));
	InitializeGate(0xDD, reinterpret_cast<uint32_t>(IDTISRDD));
	InitializeGate(0xDE, reinterpret_cast<uint32_t>(IDTISRDE));
	InitializeGate(0xDF, reinterpret_cast<uint32_t>(IDTISRDF));
	InitializeGate(0xE0, reinterpret_cast<uint32_t>(IDTISRE0));
	InitializeGate(0xE1, reinterpret_cast<uint32_t>(IDTISRE1));
	InitializeGate(0xE2, reinterpret_cast<uint32_t>(IDTISRE2));
	InitializeGate(0xE3, reinterpret_cast<uint32_t>(IDTISRE3));
	InitializeGate(0xE4, reinterpret_cast<uint32_t>(IDTISRE4));
	InitializeGate(0xE5, reinterpret_cast<uint32_t>(IDTISRE5));
	InitializeGate(0xE6, reinterpret_cast<uint32_t>(IDTISRE6));
	InitializeGate(0xE7, reinterpret_cast<uint32_t>(IDTISRE7));
	InitializeGate(0xE8, reinterpret_cast<uint32_t>(IDTISRE8));
	InitializeGate(0xE9, reinterpret_cast<uint32_t>(IDTISRE9));
	InitializeGate(0xEA, reinterpret_cast<uint32_t>(IDTISREA));
	InitializeGate(0xEB, reinterpret_cast<uint32_t>(IDTISREB));
	InitializeGate(0xEC, reinterpret_cast<uint32_t>(IDTISREC));
	InitializeGate(0xED, reinterpret_cast<uint32_t>(IDTISRED));
	InitializeGate(0xEE, reinterpret_cast<uint32_t>(IDTISREE));
	InitializeGate(0xEF, reinterpret_cast<uint32_t>(IDTISREF));
	InitializeGate(0xF0, reinterpret_cast<uint32_t>(IDTISRF0));
	InitializeGate(0xF1, reinterpret_cast<uint32_t>(IDTISRF1));
	InitializeGate(0xF2, reinterpret_cast<uint32_t>(IDTISRF2));
	InitializeGate(0xF3, reinterpret_cast<uint32_t>(IDTISRF3));
	InitializeGate(0xF4, reinterpret_cast<uint32_t>(IDTISRF4));
	InitializeGate(0xF5, reinterpret_cast<uint32_t>(IDTISRF5));
	InitializeGate(0xF6, reinterpret_cast<uint32_t>(IDTISRF6));
	InitializeGate(0xF7, reinterpret_cast<uint32_t>(IDTISRF7));
	InitializeGate(0xF8, reinterpret_cast<uint32_t>(IDTISRF8));
	InitializeGate(0xF9, reinterpret_cast<uint32_t>(IDTISRF9));
	InitializeGate(0xFA, reinterpret_cast<uint32_t>(IDTISRFA));
	InitializeGate(0xFB, reinterpret_cast<uint32_t>(IDTISRFB));
	InitializeGate(0xFC, reinterpret_cast<uint32_t>(IDTISRFC));
	InitializeGate(0xFD, reinterpret_cast<uint32_t>(IDTISRFD));
	InitializeGate(0xFE, reinterpret_cast<uint32_t>(IDTISRFE));
	InitializeGate(0xFF, reinterpret_cast<uint32_t>(IDTISRFF));
