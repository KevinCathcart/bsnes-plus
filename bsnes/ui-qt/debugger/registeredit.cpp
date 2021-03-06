#include "registeredit.moc"

RegisterEditCPU *registerEditCPU, *registerEditSA1;
RegisterEditSMP *registerEditSMP;
RegisterEditSFX *registerEditSFX;

#define reg_editor(name, digits) \
	layout->addWidget(new QLabel(QString(#name).toUpper()), layout->rowCount(), 0); \
	edit_##name = new QLineEdit(this); \
	edit_##name->setFont(QFont(Style::Monospace)); \
	edit_##name->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum); \
	edit_##name->setFixedWidth((digits + 1) * edit_##name->fontMetrics().width(' ')); \
	edit_##name->setInputMask(QString("H").repeated(digits)); \
	edit_##name->setMaxLength(digits); \
	connect(edit_##name, SIGNAL(textEdited(QString)), this, SLOT(commit())); \
	layout->addWidget(edit_##name, layout->rowCount() - 1, 1);

#define reg_sync(name, reg) \
	edit_##name->setText(QString::number(_debugger.getRegister(reg), 16) \
		.rightJustified(edit_##name->maxLength(), '0').toUpper());
#define reg_commit(name, reg) \
	if (sender() == edit_##name) { \
		bool ok; \
		int val = edit_##name->text().toInt(&ok, 16); \
		int pos = edit_##name->cursorPosition(); \
		if (ok) _debugger.setRegister(reg, val); \
		synchronize(); \
		edit_##name->setCursorPosition(pos); \
		return; \
	}

#define flag_editor(flag, num, row, column) \
	flag_btn[num] = new QCheckBox(this); \
	flag_btn[num]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum); \
	flag_btn[num]->setText(QString(flag)); \
	connect(flag_btn[num], SIGNAL(toggled(bool)), this, SLOT(commit())); \
	layout->addWidget(flag_btn[num], row, column);

#define flag_sync(flag, num) \
	flag_btn[num]->blockSignals(true); \
	flag_btn[num]->setChecked(_debugger.getFlag(flag)); \
	flag_btn[num]->blockSignals(false);
#define flag_commit(flag, num) \
	if (sender() == flag_btn[num]) { \
		_debugger.setFlag(flag, flag_btn[num]->isChecked()); \
		synchronize(); \
		return; \
	}

void RegisterEditCPU::setupUI() {
	QVBoxLayout *fullLayout = new QVBoxLayout;
	fullLayout->setSpacing(Style::TightWidgetSpacing);
	this->setLayout(fullLayout);

	QGridLayout *layout = new QGridLayout;
	layout->setHorizontalSpacing(Style::WidgetSpacing * 2);
	reg_editor(pc, 6);
	reg_editor(a, 4);
	reg_editor(x, 4);
	reg_editor(y, 4);
	reg_editor(s, 4);
	reg_editor(d, 4);
	reg_editor(db, 2);
	reg_editor(p, 2);
	fullLayout->addLayout(layout);

	fullLayout->addSpacing(Style::WidgetSpacing);

	layout = new QGridLayout;
	layout->setHorizontalSpacing(Style::WidgetSpacing * 2);
	for (int i = 0; i < 9; i++) {
		flag_editor(i["NVMXDIZCE"], i, (i >> 1), i & 1);
	}
	fullLayout->addLayout(layout);

	fullLayout->addStretch();
}

void RegisterEditCPU::commit() {
	reg_commit(pc, SNES::CPUDebugger::RegisterPC);
	reg_commit(a,  SNES::CPUDebugger::RegisterA);
	reg_commit(x,  SNES::CPUDebugger::RegisterX);
	reg_commit(y,  SNES::CPUDebugger::RegisterY);
	reg_commit(s,  SNES::CPUDebugger::RegisterS);
	reg_commit(d,  SNES::CPUDebugger::RegisterD);
	reg_commit(db, SNES::CPUDebugger::RegisterDB);
	reg_commit(p,  SNES::CPUDebugger::RegisterP);

	flag_commit(SNES::CPUDebugger::FlagN, 0);
	flag_commit(SNES::CPUDebugger::FlagV, 1);
	flag_commit(SNES::CPUDebugger::FlagM, 2);
	flag_commit(SNES::CPUDebugger::FlagX, 3);
	flag_commit(SNES::CPUDebugger::FlagD, 4);
	flag_commit(SNES::CPUDebugger::FlagI, 5);
	flag_commit(SNES::CPUDebugger::FlagZ, 6);
	flag_commit(SNES::CPUDebugger::FlagC, 7);
	flag_commit(SNES::CPUDebugger::FlagE, 8);
}

void RegisterEditCPU::synchronize() {
	reg_sync(pc, SNES::CPUDebugger::RegisterPC);
	reg_sync(a,  SNES::CPUDebugger::RegisterA);
	reg_sync(x,  SNES::CPUDebugger::RegisterX);
	reg_sync(y,  SNES::CPUDebugger::RegisterY);
	reg_sync(s,  SNES::CPUDebugger::RegisterS);
	reg_sync(d,  SNES::CPUDebugger::RegisterD);
	reg_sync(db, SNES::CPUDebugger::RegisterDB);
	reg_sync(p,  SNES::CPUDebugger::RegisterP);

	flag_sync(SNES::CPUDebugger::FlagN, 0);
	flag_sync(SNES::CPUDebugger::FlagV, 1);
	flag_sync(SNES::CPUDebugger::FlagM, 2);
	flag_sync(SNES::CPUDebugger::FlagX, 3);
	flag_sync(SNES::CPUDebugger::FlagD, 4);
	flag_sync(SNES::CPUDebugger::FlagI, 5);
	flag_sync(SNES::CPUDebugger::FlagZ, 6);
	flag_sync(SNES::CPUDebugger::FlagC, 7);
	flag_sync(SNES::CPUDebugger::FlagE, 8);
}

void RegisterEditSMP::setupUI() {
	QVBoxLayout *fullLayout = new QVBoxLayout;
	fullLayout->setSpacing(Style::TightWidgetSpacing);
	this->setLayout(fullLayout);

	QGridLayout *layout = new QGridLayout;
	layout->setHorizontalSpacing(Style::WidgetSpacing * 2);
	reg_editor(pc, 4);
	reg_editor(a, 2);
	reg_editor(x, 2);
	reg_editor(y, 2);
	reg_editor(s, 2);
	reg_editor(ya, 4);
	reg_editor(p, 2);
	fullLayout->addLayout(layout);

	fullLayout->addSpacing(Style::WidgetSpacing);

	layout = new QGridLayout;
	layout->setHorizontalSpacing(Style::WidgetSpacing * 2);
	for (int i = 0; i < 8; i++) {
		flag_editor(i["NVPBHIZC"], i, i >> 1, i & 1);
	}
	fullLayout->addLayout(layout);

	fullLayout->addStretch();
}

void RegisterEditSMP::commit() {
	reg_commit(pc, SNES::SMPDebugger::RegisterPC);
	reg_commit(a,  SNES::SMPDebugger::RegisterA);
	reg_commit(x,  SNES::SMPDebugger::RegisterX);
	reg_commit(y,  SNES::SMPDebugger::RegisterY);
	reg_commit(s,  SNES::SMPDebugger::RegisterS);
	reg_commit(ya, SNES::SMPDebugger::RegisterYA);
	reg_commit(p,  SNES::SMPDebugger::RegisterP);

	flag_commit(SNES::SMPDebugger::FlagN, 0);
	flag_commit(SNES::SMPDebugger::FlagV, 1);
	flag_commit(SNES::SMPDebugger::FlagP, 2);
	flag_commit(SNES::SMPDebugger::FlagB, 3);
	flag_commit(SNES::SMPDebugger::FlagH, 4);
	flag_commit(SNES::SMPDebugger::FlagI, 5);
	flag_commit(SNES::SMPDebugger::FlagZ, 6);
	flag_commit(SNES::SMPDebugger::FlagC, 7);
}

void RegisterEditSMP::synchronize() {
	reg_sync(pc, SNES::SMPDebugger::RegisterPC);
	reg_sync(a,  SNES::SMPDebugger::RegisterA);
	reg_sync(x,  SNES::SMPDebugger::RegisterX);
	reg_sync(y,  SNES::SMPDebugger::RegisterY);
	reg_sync(s,  SNES::SMPDebugger::RegisterS);
	reg_sync(ya, SNES::SMPDebugger::RegisterYA);
	reg_sync(p,  SNES::SMPDebugger::RegisterP);

	flag_sync(SNES::SMPDebugger::FlagN, 0);
	flag_sync(SNES::SMPDebugger::FlagV, 1);
	flag_sync(SNES::SMPDebugger::FlagP, 2);
	flag_sync(SNES::SMPDebugger::FlagB, 3);
	flag_sync(SNES::SMPDebugger::FlagH, 4);
	flag_sync(SNES::SMPDebugger::FlagI, 5);
	flag_sync(SNES::SMPDebugger::FlagZ, 6);
	flag_sync(SNES::SMPDebugger::FlagC, 7);
}

void RegisterEditSFX::setupUI() {
	QVBoxLayout *fullLayout = new QVBoxLayout;
	fullLayout->setSpacing(Style::TightWidgetSpacing);
	this->setLayout(fullLayout);

	QGridLayout *layout = new QGridLayout;
	layout->setHorizontalSpacing(Style::WidgetSpacing * 2);
	for (int reg = 0; reg < 16; reg++) {
		layout->addWidget(new QLabel(QString("R%1").arg(reg)), reg>>1, (reg&1)<<1);
		edit_r[reg] = new QLineEdit(this);
		edit_r[reg]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
		edit_r[reg]->setFont(QFont(Style::Monospace));
		edit_r[reg]->setFixedWidth((4 + 1) * edit_r[reg]->fontMetrics().width(' '));
		edit_r[reg]->setInputMask("HHHH");
		edit_r[reg]->setMaxLength(4);
		connect(edit_r[reg], SIGNAL(textEdited(QString)), this, SLOT(commit()));
		layout->addWidget(edit_r[reg], reg>>1, ((reg&1)<<1) + 1);
	}
	reg_editor(sfr, 4);
	fullLayout->addLayout(layout);

	fullLayout->addSpacing(Style::WidgetSpacing);

	layout = new QGridLayout;
	layout->setHorizontalSpacing(Style::WidgetSpacing * 2);
	// TODO: some other registers here (ROMBR, etc)
	flag_editor("N",    0,  0, 0);
	flag_editor("V",    1,  0, 1);
	flag_editor("Z",    2,  1, 0);
	flag_editor("C",    3,  1, 1);
	flag_editor("G",    4,  2, 0);
	flag_editor("R",    5,  2, 1);
	flag_editor("ALT1", 6,  3, 0);
	flag_editor("ALT2", 7,  3, 1);
	flag_editor("IL",   8,  4, 0);
	flag_editor("IH",   9,  4, 1);
	flag_editor("B",    10, 5, 0);
	flag_editor("IRQ",  11, 5, 1);

	//layout->addStretch();
	fullLayout->addLayout(layout);
}

void RegisterEditSFX::commit() {
	for (int i = 0; i < 16; i++) {
		reg_commit(r[i], i);
	}
	reg_commit(sfr, SNES::SFXDebugger::RegisterSFR);

	flag_commit(SNES::SFXDebugger::FlagN,  0);
	flag_commit(SNES::SFXDebugger::FlagV,  1);
	flag_commit(SNES::SFXDebugger::FlagZ,  2);
	flag_commit(SNES::SFXDebugger::FlagC,  3);
	flag_commit(SNES::SFXDebugger::FlagG,  4);
	flag_commit(SNES::SFXDebugger::FlagR,  5);
	flag_commit(SNES::SFXDebugger::FlagA1, 6);
	flag_commit(SNES::SFXDebugger::FlagA2, 7);
	flag_commit(SNES::SFXDebugger::FlagIL, 8);
	flag_commit(SNES::SFXDebugger::FlagIH, 9);
	flag_commit(SNES::SFXDebugger::FlagB,  10);
	flag_commit(SNES::SFXDebugger::FlagI,  11);
}

void RegisterEditSFX::synchronize() {
	for (int i = 0; i < 16; i++) {
		reg_sync(r[i], i);
	}
	reg_sync(sfr, SNES::SFXDebugger::RegisterSFR);

	flag_sync(SNES::SFXDebugger::FlagN,  0);
	flag_sync(SNES::SFXDebugger::FlagV,  1);
	flag_sync(SNES::SFXDebugger::FlagZ,  2);
	flag_sync(SNES::SFXDebugger::FlagC,  3);
	flag_sync(SNES::SFXDebugger::FlagG,  4);
	flag_sync(SNES::SFXDebugger::FlagR,  5);
	flag_sync(SNES::SFXDebugger::FlagA1, 6);
	flag_sync(SNES::SFXDebugger::FlagA2, 7);
	flag_sync(SNES::SFXDebugger::FlagIL, 8);
	flag_sync(SNES::SFXDebugger::FlagIH, 9);
	flag_sync(SNES::SFXDebugger::FlagB,  10);
	flag_sync(SNES::SFXDebugger::FlagI,  11);
}

#undef reg_editor
#undef reg_sync
#undef reg_commit
#undef flag_editor
#undef flag_sync
#undef flag_commit
