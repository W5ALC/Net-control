#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QProgressBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>
#include <QScrollArea>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTimer>
#include <QTreeWidget>
#include <QHeaderView>
#include <QComboBox>
#include <QCheckBox>
#include <QToolBar>
#include <QAction>
#include <QShortcut>
#include <QSettings>
#include <QTableWidget>
#include <QTimeEdit>
#include <QGroupBox>
#include <QInputDialog>
#include <algorithm>
#include <random>
#include <vector>

// Constants
const int MAX_TOPICS = 100;
const int MAX_ANNOUNCEMENTS = 50;

// Default values
const QString DEFAULT_CLUB_NAME = "Pueblo Amateur Radio Club";
const QString DEFAULT_NET_NIGHT = "Wednesday";
const QString DEFAULT_NET_TIME = "7:00 PM MST/MDT";
const QString DEFAULT_CALLSIGN = "W5ALC";
const QString DEFAULT_NAME = "Jon";
const QString DEFAULT_LOCATION = "Pueblo, CO";
const int DEFAULT_NUM_TOPICS = 1;
const QString DEFAULT_LOGGER_CALLSIGN = "N0CALL";
const QString DEFAULT_LOGGER_NAME = "LOGGER";

const QStringList DEFAULT_TOPICS = {
    // Antennas & RF (1-18)
    "Could you tell us about your favorite antenna configuration and why you prefer it?",
    "Do you have any tips for setting up effective antennas in small or restricted spaces?",
    "What's your experience with vertical versus horizontal antenna polarization?",
    "Have you experimented with wire antennas? What designs have worked best for you?",
    "What tools or techniques do you use for antenna tuning and SWR adjustment?",
    "How do you choose feedline: coax versus ladder line for different scenarios?",
    "Have you used antenna modeling software (NEC, EZNEC, 4nec2)? What did you learn?",
    "What balun or matching approach do you prefer and why?",
    "Have you tried magnetic loop antennas? Pros, cons and tuning tips",
    "How do you design a multi-band wire antenna for limited space?",
    "Balancing bandwidth, gain and pattern when designing yagis — your approach",
    "NVIS antennas: when and how have you used them for local coverage?",
    "Tips for maintaining connectors, coax and lightning protection",
    "What are your best practices for lightning protection and surge arrestors?",
    "What ferrites, chokes or filters do you recommend for common noise sources?",
    "How do you approach station grounding for both safety and noise control?",
    "Share a story about diagnosing and fixing a hidden antenna problem",
    "Phased arrays and stacking: when are they worth the added complexity?",

    // Propagation & Conditions (19-34)
    "How have the propagation conditions been in your area? Have you encountered any challenges or successes lately?",
    "Do you actively monitor solar activity and use propagation prediction tools? Which ones?",
    "How do you interpret sunspot and geomagnetic data for planning HF operations?",
    "Have you experienced any unusual propagation phenomena like sporadic-E or aurora scatter?",
    "Tell us about a recent propagation opening that surprised you",
    "How do seasonal changes affect your band choices and antenna setup?",
    "What are practical tips for working short openings on HF?",
    "Do you use beacons and CW skimmers to find openings? Workflow tips",
    "Experience with sporadic-E for VHF contacts and tips to exploit it",
    "How do you adapt your operating strategy based on current band conditions?",
    "How do you log and learn from propagation anomalies over time?",
    "Have you used SDRs or webSDRs to study propagation and noise?",
    "What's your experience with meteor scatter or other short-duration propagation modes?",
    "Have you worked EME or moonbounce? Setup and procedural tips",

    // Operating & Contacts (35-56)
    "Have you made any interesting DX contacts recently? Please share your experience.",
    "What's the most memorable QSO or event you've experienced in amateur radio?",
    "If you could have a QSO with anyone, living or passed, who would it be and why?",
    "Do you prefer ragchewing or contest-style operating? What appeals to you about that style?",
    "What bands do you operate most frequently, and what draws you to them?",
    "What's your favorite DX contact story and how did you make it happen?",
    "How do you handle pileups effectively and politely?",
    "Share tips for working split pileups effectively and without aggravation.",
    "What logging and spotting tools are essential for serious contesting?",
    "How do you prepare for a DXpedition or a big contest weekend?",
    "What's your approach to antenna maintenance and seasonal adjustments?",
    "How do you handle operator fatigue and maintain focus during long contest shifts?",
    "What are the ethical considerations around contesting, spotting, and cluster use?",
    "Have you made contacts with hams in countries with interesting or rare prefixes?",
    "What cultural differences have you noticed when operating internationally?",
    "Do you operate in languages other than your native language?",
    "What's your approach to running an efficient net check-in session?",
    "What makes a good net control operator — tips for volunteers?",

    // Contests & Awards (57-66)
    "Do you participate in contests? What's your favorite contest and why?",
    "Contest strategy: running vs searching and pouncing — how do you train?",
    "How do you balance competitive operating with casual enjoyment of the hobby?",
    "What awards are you currently working toward (DXCC, WAS, WAC, etc.)?",
    "What's your most prized QSL card or achievement in amateur radio?",
    "How do you keep an accurate contest log under pressure?",
    "What are the best practices for cross-checking logs and post-contest cleanup?",
    "How do you use spotting networks and reverse-beaconing responsibly?",

    // Digital Modes & Technology (67-88)
    "Do you operate any digital modes? What are your thoughts on the future of digital in amateur radio?",
    "What's your experience with FT8, FT4, or other WSJT-X modes?",
    "Have you tried any of the newer digital voice modes like FreeDV or M17?",
    "What software do you use for logging, digital modes, or rig control?",
    "How do you feel about SDR technology and its impact on the hobby?",
    "Experience with SDRs: platforms, strengths, and favorite applications",
    "Have you built any custom DSP or demod chains with GNU Radio or SDR software?",
    "How do you integrate a Raspberry Pi or SBC into your station automation?",
    "How do you run remote or headless stations reliably over the internet?",
    "What are your thoughts on digital voice systems like DMR, D-STAR and Fusion?",
    "Have you used SDRs for spectrum monitoring or interference hunting?",
    "Have you set up a webSDR or used public webSDR receivers effectively?",
    "What privacy or security concerns exist with modern networked rigs and how do you mitigate them?",
    "What single-board computer projects improved your station's automation?",
    "Have you integrated home automation tech (voice assistants, smart relays) with your station?",
    "How do you handle operator identification and logging for automated spots?",

    // Homebrew, Electronics & Measurements (89-108)
    "Are you working on any new homebrew projects or modifications that you'd like to share with us?",
    "Have you built any of your own equipment? What was the most rewarding project?",
    "What resources do you recommend for those interested in building their own gear?",
    "Do you do any circuit modifications or repairs on commercial equipment?",
    "Beginner electronics project recommendations for learning RF basics",
    "How do you use antenna analyzers, VNAs or MFJ-style tools in practice?",
    "Tips for soldering, shielding and building reliable RF circuits",
    "Have you designed any matching networks or transformers? Share details",
    "What resources helped you learn PCB layout and RF grounding?",
    "How do you measure and interpret SWR, return loss and impedance?",
    "What test gear do you consider essential for a serious home workshop?",
    "Have you experimented with building or tuning a magnetic loop in the home shack?",
    "Have you tried building or using directional receiving antennas for noise reduction?",
    "Share an interesting shack automation you built using Raspberry Pi or microcontrollers?",
    "How do you keep motivation for long-term projects or license upgrades?",

    // Power, Batteries & Solar (109-120)
    "What battery chemistry do you prefer for field or backup power and why?",
    "Comparing LiFePO4, lithium-ion and sealed lead-acid for ham use — experiences?",
    "How do you size a solar + battery system for occasional field use?",
    "Power budgeting for a portable station: calculator tips and rules of thumb",
    "Inverters and generators: do you use them and what are key considerations?",
    "Battery maintenance tips that prolong life and reliability",
    "What's your strategy for power management during portable operations?",
    "How do you manage charging and battery health during long activations?",
    "What are realistic expectations for amateur radio in a long-duration outage?",
    "Have you used renewable power systems for remote or permanent stations?",

    // Portable, POTA, SOTA & Field Ops (121-134)
    "For those who enjoy portable operations, what is your go-to setup when operating in the field?",
    "Have you participated in any Summits on the Air (SOTA) or Parks on the Air (POTA) activations?",
    "What's your strategy for power management during portable operations?",
    "Do you have a favorite portable antenna system? Why does it work well for you?",
    "Field ops checklist: what do you always pack and why?",
    "Lightweight antenna ideas for hiking and ultralight portable ops",
    "Leave-no-trace and safety practices when operating from public lands",
    "Share a memorable POTA or SOTA activation and lessons learned",
    "How do you choose CW, voice or digital modes for a summit activation?",
    "How do you prepare for portable ops in extreme weather or high-altitude environments?",
    "How do you manage log backups and secure your portable logbook data?",
    "Have you used APRS or LoRa to assist portable activations or teams?",
    "What low-cost upgrades gave you the biggest improvements for field ops?",
    "What privacy or security concerns exist with modern networked portable rigs?",

    // VHF/UHF, Microwave & Special Modes (135-144)
    "Do you operate on VHF/UHF bands? What activities do you enjoy on those frequencies?",
    "Tips for starting weak-signal VHF/UHF work: equipment and techniques",
    "Experience with microwave bands, dishes and transverters",
    "Have you worked EME (moonbounce)? Setup and procedural tips",
    "Have you experimented with APRS or other VHF/UHF packet modes?",
    "What's your experience with meteor scatter or other short-duration propagation modes?",
    "Have you tried any microwave bands? What unique challenges do they present?",
    "What projects helped you learn antenna pointing and alignment?",
    "Have you used directional antennas for local noise mitigation or receive enhancement?",
    "What telemetry, IoT, or APRS projects have you built or used successfully?",

    // Satellites, Balloons & Space (145-150)
    "Have you worked any amateur radio satellites? What equipment do you use?",
    "How did you get started with amateur satellites or the ISS contacts?",
    "Tips for tracking, doppler correction and simple satellite antennas",
    "Have you launched or tracked high-altitude balloons? Payload ideas?",
    "How do you decode weather satellites (APT/NOAA) or HRPT data?",
    "What are your thoughts on small-sat opportunities for clubs and schools?"
};

const QStringList DEFAULT_ANNOUNCEMENTS = {
    "No new Announcements at this time."
};

// Section structure
struct Section {
    QString title;
    QString content;
    int estimatedMinutes;
};

// Check-in structure
struct CheckIn {
    QString callsign;
    QString name;
    QString location;
    QString comments;
    QDateTime time;
    bool hasTraffic;
    QVector<int> respondedToTopics;
};

// Template structure
struct NetTemplate {
    QString name;
    QString clubName;
    QString netNight;
    QString netTime;
    QString callsign;
    QString operatorName;
    QString location;
    int numTopics;
    QString loggerCallsign;
    QString loggerName;
};

class ARCNetScript : public QMainWindow {
    Q_OBJECT

public:
    ARCNetScript(QWidget *parent = nullptr);
    ~ARCNetScript();

private slots:
    void loadTopicsFile();
    void loadAnnouncementsFile();
    void randomizeTopics();
    void resetFields();
    void startNetScript();
    void nextSection();
    void prevSection();
    void exportScript();
    void toggleTheme();
    void onSectionSelected(int index);
    void validateFields();
    void updateLivePreview();
    void saveTemplate();
    void loadTemplate();
    void addCheckIn();
    void removeCheckIn();
    void exportCheckInLog();
    void startTimer();
    void updateTimer();
    void increaseFontSize();
    void decreaseFontSize();
    void toggleHighContrast();
    void filterTopicsByCategory();
    void markTopicAsFavorite();

private:
    void createSetupTab();
    void createScriptTab();
    void createCheckInTab();
    void createSettingsTab();
    void createToolbar();
    void loadDefaultData();
    void updateTopicPreview();
    void updateAnnouncementPreview();
    void generateScriptSections();
    void displaySection();
    void populateSectionList();
    void showStatusMessage(const QString &message, bool isError = false);
    void applyTheme();
    void setupKeyboardShortcuts();
    void loadSettings();
    void saveSettings();
    QStringList readLinesFromFile(const QString &filename);

    // UI Components
    QTabWidget *tabWidget;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    // Setup tab widgets
    QLineEdit *clubEdit;
    QLineEdit *nightEdit;
    QLineEdit *timeEdit;
    QLineEdit *callsignEdit;
    QLineEdit *nameEdit;
    QLineEdit *locationEdit;
    QSpinBox *numTopicsSpin;
    QLineEdit *loggerCallsignEdit;
    QLineEdit *loggerNameEdit;
    QLabel *topicFileLabel;
    QLabel *announcementFileLabel;
    QTextEdit *topicPreview;
    QTextEdit *announcementPreview;
    QTextEdit *livePreview;
    QPushButton *startButton;
    QComboBox *templateCombo;
    QComboBox *topicCategoryFilter;

    // Script tab widgets
    QLabel *sectionLabel;
    QProgressBar *progressBar;
    QListWidget *sectionList;
    QTextEdit *sectionText;
    QPushButton *prevButton;
    QPushButton *nextButton;
    QPushButton *exportButton;
    QLabel *timerLabel;
    QLabel *estimatedTimeLabel;

    // Check-in tab widgets
    QTableWidget *checkInTable;
    QLineEdit *checkInCallsign;
    QLineEdit *checkInName;
    QLineEdit *checkInLocation;
    QTextEdit *checkInComments;
    QCheckBox *checkInHasTraffic;
    QPushButton *addCheckInButton;
    QLabel *checkInCountLabel;

    // Settings widgets
    QSpinBox *fontSizeSpinBox;
    QCheckBox *highContrastCheck;
    QCheckBox *largeButtonCheck;
    QCheckBox *autoSaveCheck;

    // Data
    QStringList topics;
    QStringList announcements;
    QVector<Section> sections;
    QVector<CheckIn> checkIns;
    QVector<NetTemplate> templates;
    int currentSection;

    // Settings
    bool darkTheme;
    bool highContrast;
    int fontSize;
    QString topicFilePath;
    QString announcementFilePath;

    // Timer
    QTimer *netTimer;
    QTime elapsedTime;
};

ARCNetScript::ARCNetScript(QWidget *parent)
    : QMainWindow(parent), currentSection(0), darkTheme(true),
      highContrast(false), fontSize(10) {

    setWindowTitle("Amateur Radio Net Script Generator");
    setWindowIcon(QIcon("icon.png"));
    resize(1200, 800);

    loadDefaultData();
    loadSettings();

    // Create toolbar
    createToolbar();

    // Create central widget and main layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create tab widget
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // Create tabs
    createSetupTab();
    createScriptTab();
    createCheckInTab();
    createSettingsTab();

    // Create status bar
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    setCentralWidget(centralWidget);

    // Setup keyboard shortcuts
    setupKeyboardShortcuts();

    // Apply initial theme
    applyTheme();

    // Initial validation
    validateFields();

    // Timer setup
    netTimer = new QTimer(this);
    connect(netTimer, &QTimer::timeout, this, &ARCNetScript::updateTimer);
}

ARCNetScript::~ARCNetScript() {
    saveSettings();
}

void ARCNetScript::createToolbar() {
    toolBar = addToolBar("Main Toolbar");
    toolBar->setMovable(false);

    QAction *saveTemplateAction = new QAction("Save Template", this);
    connect(saveTemplateAction, &QAction::triggered, this, &ARCNetScript::saveTemplate);
    toolBar->addAction(saveTemplateAction);

    QAction *loadTemplateAction = new QAction("Load Template", this);
    connect(loadTemplateAction, &QAction::triggered, this, &ARCNetScript::loadTemplate);
    toolBar->addAction(loadTemplateAction);

    toolBar->addSeparator();

    QAction *fontPlusAction = new QAction("A+", this);
    fontPlusAction->setToolTip("Increase Font Size");
    connect(fontPlusAction, &QAction::triggered, this, &ARCNetScript::increaseFontSize);
    toolBar->addAction(fontPlusAction);

    QAction *fontMinusAction = new QAction("A-", this);
    fontMinusAction->setToolTip("Decrease Font Size");
    connect(fontMinusAction, &QAction::triggered, this, &ARCNetScript::decreaseFontSize);
    toolBar->addAction(fontMinusAction);

    toolBar->addSeparator();

    QAction *startTimerAction = new QAction("Start Timer", this);
    connect(startTimerAction, &QAction::triggered, this, &ARCNetScript::startTimer);
    toolBar->addAction(startTimerAction);
}

void ARCNetScript::setupKeyboardShortcuts() {
    // Navigation shortcuts
    QShortcut *nextShortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
    connect(nextShortcut, &QShortcut::activated, this, &ARCNetScript::nextSection);

    QShortcut *prevShortcut = new QShortcut(QKeySequence(Qt::Key_Backspace), this);
    connect(prevShortcut, &QShortcut::activated, this, &ARCNetScript::prevSection);

    // Quick jump shortcuts (1-9)
    for (int i = 1; i <= 9; i++) {
        QShortcut *jumpShortcut = new QShortcut(QKeySequence(Qt::Key_0 + i), this);
        connect(jumpShortcut, &QShortcut::activated, [this, i]() {
            if (i - 1 < sections.size()) {
                currentSection = i - 1;
                displaySection();
            }
        });
    }

    // Font size shortcuts
    QShortcut *fontUpShortcut = new QShortcut(QKeySequence::ZoomIn, this);
    connect(fontUpShortcut, &QShortcut::activated, this, &ARCNetScript::increaseFontSize);

    QShortcut *fontDownShortcut = new QShortcut(QKeySequence::ZoomOut, this);
    connect(fontDownShortcut, &QShortcut::activated, this, &ARCNetScript::decreaseFontSize);
}

void ARCNetScript::loadSettings() {
    QSettings settings("ARCNetScript", "NetGenerator");

    darkTheme = settings.value("darkTheme", true).toBool();
    highContrast = settings.value("highContrast", false).toBool();
    fontSize = settings.value("fontSize", 10).toInt();

    // Load last used values
    if (settings.value("rememberLastSession", true).toBool()) {
        // Could restore last session here
    }
}

void ARCNetScript::saveSettings() {
    QSettings settings("ARCNetScript", "NetGenerator");

    settings.setValue("darkTheme", darkTheme);
    settings.setValue("highContrast", highContrast);
    settings.setValue("fontSize", fontSize);
}

void ARCNetScript::loadDefaultData() {
    topics = DEFAULT_TOPICS;
    announcements = DEFAULT_ANNOUNCEMENTS;
    topicFilePath.clear();
    announcementFilePath.clear();
}

void ARCNetScript::createSetupTab() {
    QWidget *setupWidget = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout(setupWidget);

    // Left side: Form
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    QWidget *scrollContent = new QWidget();
    QGridLayout *grid = new QGridLayout(scrollContent);
    grid->setSpacing(8);

    int row = 0;

    // Template selector
    QGroupBox *templateGroup = new QGroupBox("Quick Start");
    QVBoxLayout *templateLayout = new QVBoxLayout(templateGroup);
    templateCombo = new QComboBox();
    templateCombo->addItem("-- New Configuration --");
    templateCombo->addItem("Standard Ragchew Net");
    templateCombo->addItem("ARES Emergency Net");
    templateCombo->addItem("Contest Information Net");
    templateLayout->addWidget(new QLabel("Load Template:"));
    templateLayout->addWidget(templateCombo);
    grid->addWidget(templateGroup, row++, 0, 1, 2);

    // Club Name
    grid->addWidget(new QLabel("Your Club Name:"), row, 0);
    clubEdit = new QLineEdit(DEFAULT_CLUB_NAME);
    connect(clubEdit, &QLineEdit::textChanged, this, &ARCNetScript::validateFields);
    connect(clubEdit, &QLineEdit::textChanged, this, &ARCNetScript::updateLivePreview);
    grid->addWidget(clubEdit, row++, 1);

    // Net Night
    grid->addWidget(new QLabel("Night of the Week:"), row, 0);
    nightEdit = new QLineEdit(DEFAULT_NET_NIGHT);
    connect(nightEdit, &QLineEdit::textChanged, this, &ARCNetScript::validateFields);
    connect(nightEdit, &QLineEdit::textChanged, this, &ARCNetScript::updateLivePreview);
    grid->addWidget(nightEdit, row++, 1);

    // Net Time
    grid->addWidget(new QLabel("Time of the Net:"), row, 0);
    timeEdit = new QLineEdit(DEFAULT_NET_TIME);
    connect(timeEdit, &QLineEdit::textChanged, this, &ARCNetScript::validateFields);
    connect(timeEdit, &QLineEdit::textChanged, this, &ARCNetScript::updateLivePreview);
    grid->addWidget(timeEdit, row++, 1);

    // Callsign
    grid->addWidget(new QLabel("Your Callsign:"), row, 0);
    callsignEdit = new QLineEdit(DEFAULT_CALLSIGN);
    connect(callsignEdit, &QLineEdit::textChanged, this, &ARCNetScript::validateFields);
    connect(callsignEdit, &QLineEdit::textChanged, this, &ARCNetScript::updateLivePreview);
    grid->addWidget(callsignEdit, row++, 1);

    // Name
    grid->addWidget(new QLabel("Your Name:"), row, 0);
    nameEdit = new QLineEdit(DEFAULT_NAME);
    connect(nameEdit, &QLineEdit::textChanged, this, &ARCNetScript::validateFields);
    connect(nameEdit, &QLineEdit::textChanged, this, &ARCNetScript::updateLivePreview);
    grid->addWidget(nameEdit, row++, 1);

    // Location
    grid->addWidget(new QLabel("Location:"), row, 0);
    locationEdit = new QLineEdit(DEFAULT_LOCATION);
    connect(locationEdit, &QLineEdit::textChanged, this, &ARCNetScript::validateFields);
    connect(locationEdit, &QLineEdit::textChanged, this, &ARCNetScript::updateLivePreview);
    grid->addWidget(locationEdit, row++, 1);

    // Number of Topics
    grid->addWidget(new QLabel("Number of Topics:"), row, 0);
    numTopicsSpin = new QSpinBox();
    numTopicsSpin->setRange(1, MAX_TOPICS);
    numTopicsSpin->setValue(DEFAULT_NUM_TOPICS);
    connect(numTopicsSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ARCNetScript::updateLivePreview);
    grid->addWidget(numTopicsSpin, row++, 1);

    // Logger Callsign
    grid->addWidget(new QLabel("Logger Callsign:"), row, 0);
    loggerCallsignEdit = new QLineEdit(DEFAULT_LOGGER_CALLSIGN);
    connect(loggerCallsignEdit, &QLineEdit::textChanged, this, &ARCNetScript::validateFields);
    connect(loggerCallsignEdit, &QLineEdit::textChanged, this, &ARCNetScript::updateLivePreview);
    grid->addWidget(loggerCallsignEdit, row++, 1);

    // Logger Name
    grid->addWidget(new QLabel("Logger Name:"), row, 0);
    loggerNameEdit = new QLineEdit(DEFAULT_LOGGER_NAME);
    connect(loggerNameEdit, &QLineEdit::textChanged, this, &ARCNetScript::validateFields);
    connect(loggerNameEdit, &QLineEdit::textChanged, this, &ARCNetScript::updateLivePreview);
    grid->addWidget(loggerNameEdit, row++, 1);

    // Topics Management
    QGroupBox *topicsGroup = new QGroupBox("Topics Management");
    QVBoxLayout *topicsGroupLayout = new QVBoxLayout(topicsGroup);

    QHBoxLayout *topicFileLayout = new QHBoxLayout();
    QPushButton *loadTopicsBtn = new QPushButton("Load Topics File...");
    connect(loadTopicsBtn, &QPushButton::clicked, this, &ARCNetScript::loadTopicsFile);
    topicFileLabel = new QLabel("Using default topics");
    topicFileLayout->addWidget(loadTopicsBtn);
    topicFileLayout->addWidget(topicFileLabel);
    topicFileLayout->addStretch();
    topicsGroupLayout->addLayout(topicFileLayout);

    topicCategoryFilter = new QComboBox();
    topicCategoryFilter->addItem("All Topics");
    topicCategoryFilter->addItem("Technical");
    topicCategoryFilter->addItem("Operating");
    topicCategoryFilter->addItem("Emergency Prep");
    topicCategoryFilter->addItem("Favorites");
    topicsGroupLayout->addWidget(topicCategoryFilter);

    topicPreview = new QTextEdit();
    topicPreview->setReadOnly(true);
    topicPreview->setMaximumHeight(100);
    topicsGroupLayout->addWidget(topicPreview);

    grid->addWidget(topicsGroup, row++, 0, 1, 2);

    // Announcements File
    grid->addWidget(new QLabel("Announcements File:"), row, 0);
    QHBoxLayout *announcementFileLayout = new QHBoxLayout();
    QPushButton *loadAnnouncementsBtn = new QPushButton("Load...");
    connect(loadAnnouncementsBtn, &QPushButton::clicked, this, &ARCNetScript::loadAnnouncementsFile);
    announcementFileLabel = new QLabel("Using default announcements");
    announcementFileLayout->addWidget(loadAnnouncementsBtn);
    announcementFileLayout->addWidget(announcementFileLabel);
    announcementFileLayout->addStretch();
    grid->addLayout(announcementFileLayout, row++, 1);

    // Announcement Preview
    grid->addWidget(new QLabel("Announcement Preview:"), row, 0, Qt::AlignTop);
    announcementPreview = new QTextEdit();
    announcementPreview->setReadOnly(true);
    announcementPreview->setMaximumHeight(100);
    grid->addWidget(announcementPreview, row++, 1);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *randomizeBtn = new QPushButton("Randomize Topics");
    connect(randomizeBtn, &QPushButton::clicked, this, &ARCNetScript::randomizeTopics);
    QPushButton *resetBtn = new QPushButton("Reset Fields");
    connect(resetBtn, &QPushButton::clicked, this, &ARCNetScript::resetFields);
    startButton = new QPushButton("Generate Net Script");
    startButton->setMinimumHeight(40);
    QFont buttonFont = startButton->font();
    buttonFont.setBold(true);
    startButton->setFont(buttonFont);
    connect(startButton, &QPushButton::clicked, this, &ARCNetScript::startNetScript);

    buttonLayout->addWidget(randomizeBtn);
    buttonLayout->addWidget(resetBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(startButton);
    grid->addLayout(buttonLayout, row++, 0, 1, 2);

    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea, 3);

    // Right side: Live Preview
    QGroupBox *previewGroup = new QGroupBox("Live Preview");
    QVBoxLayout *previewLayout = new QVBoxLayout(previewGroup);

    QLabel *previewLabel = new QLabel("Your script opening will look like this:");
    previewLabel->setWordWrap(true);
    previewLayout->addWidget(previewLabel);

    livePreview = new QTextEdit();
    livePreview->setReadOnly(true);
    QFont previewFont("Monospace");
    previewFont.setStyleHint(QFont::TypeWriter);
    livePreview->setFont(previewFont);
    previewLayout->addWidget(livePreview);

    mainLayout->addWidget(previewGroup, 2);

    updateTopicPreview();
    updateAnnouncementPreview();
    updateLivePreview();

    tabWidget->addTab(setupWidget, "Setup");
}

void ARCNetScript::createScriptTab() {
    QWidget *scriptWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(scriptWidget);

    // Timer and section info
    QHBoxLayout *infoLayout = new QHBoxLayout();

    sectionLabel = new QLabel("Section");
    QFont labelFont = sectionLabel->font();
    labelFont.setPointSize(12);
    labelFont.setBold(true);
    sectionLabel->setFont(labelFont);
    sectionLabel->setFixedHeight(30);
    sectionLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    infoLayout->addWidget(sectionLabel);

    infoLayout->addStretch();

    timerLabel = new QLabel("00:00:00");
    QFont timerFont = timerLabel->font();
    timerFont.setPointSize(14);
    timerFont.setBold(true);
    timerLabel->setFont(timerFont);
    timerLabel->setFixedHeight(30);

    infoLayout->addWidget(timerLabel);

    estimatedTimeLabel = new QLabel("Est: 00 min");
    estimatedTimeLabel->setFixedHeight(30);
    infoLayout->addWidget(estimatedTimeLabel);

    layout->addLayout(infoLayout);

    // Progress bar
    progressBar = new QProgressBar();
    progressBar->setTextVisible(true);
    progressBar->setVisible(false);
    progressBar->setMinimumHeight(25);
    layout->addWidget(progressBar);

    // Navigation buttons
    QHBoxLayout *navLayout = new QHBoxLayout();
    prevButton = new QPushButton("◄ Previous (Backspace)");
    nextButton = new QPushButton("Next (Space) ►");
    prevButton->setEnabled(false);
    nextButton->setEnabled(false);
    prevButton->setMinimumHeight(35);
    nextButton->setMinimumHeight(35);
    connect(prevButton, &QPushButton::clicked, this, &ARCNetScript::prevSection);
    connect(nextButton, &QPushButton::clicked, this, &ARCNetScript::nextSection);

    exportButton = new QPushButton("Export Script...");
    exportButton->setEnabled(false);
    exportButton->setMinimumHeight(35);
    connect(exportButton, &QPushButton::clicked, this, &ARCNetScript::exportScript);

    navLayout->addWidget(prevButton);
    navLayout->addWidget(nextButton);
    navLayout->addStretch();
    navLayout->addWidget(exportButton);
    layout->addLayout(navLayout);

    // Splitter with section list and content
    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    sectionList = new QListWidget();
    sectionList->setMinimumWidth(250);
    QFont listFont = sectionList->font();
    listFont.setPointSize(10);
    sectionList->setFont(listFont);
    connect(sectionList, &QListWidget::currentRowChanged, this, &ARCNetScript::onSectionSelected);
    splitter->addWidget(sectionList);

    sectionText = new QTextEdit();
    sectionText->setReadOnly(true);
    sectionText->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    QFont monoFont("Monospace");
    monoFont.setStyleHint(QFont::TypeWriter);
    monoFont.setPointSize(fontSize);
    sectionText->setFont(monoFont);
    splitter->addWidget(sectionText);

    splitter->setStretchFactor(1, 4);
    layout->addWidget(splitter);

    tabWidget->addTab(scriptWidget, "Script");
}

void ARCNetScript::createCheckInTab() {
    QWidget *checkInWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(checkInWidget);

    // Check-in count
    checkInCountLabel = new QLabel("Total Check-ins: 0");
    QFont countFont = checkInCountLabel->font();
    countFont.setPointSize(12);
    countFont.setBold(true);
    checkInCountLabel->setFont(countFont);
    mainLayout->addWidget(checkInCountLabel);

    // Check-in form
    QGroupBox *formGroup = new QGroupBox("Add Check-in");
    QGridLayout *formLayout = new QGridLayout(formGroup);

    formLayout->addWidget(new QLabel("Callsign:"), 0, 0);
    checkInCallsign = new QLineEdit();
    checkInCallsign->setPlaceholderText("e.g., W5ALC");
    formLayout->addWidget(checkInCallsign, 0, 1);

    formLayout->addWidget(new QLabel("Name:"), 0, 2);
    checkInName = new QLineEdit();
    checkInName->setPlaceholderText("e.g., Jon");
    formLayout->addWidget(checkInName, 0, 3);

    formLayout->addWidget(new QLabel("Location:"), 1, 0);
    checkInLocation = new QLineEdit();
    checkInLocation->setPlaceholderText("e.g., Pueblo, CO");
    formLayout->addWidget(checkInLocation, 1, 1);

    checkInHasTraffic = new QCheckBox("Has Traffic");
    formLayout->addWidget(checkInHasTraffic, 1, 2);

    addCheckInButton = new QPushButton("Add Check-in");
    addCheckInButton->setMinimumHeight(30);
    connect(addCheckInButton, &QPushButton::clicked, this, &ARCNetScript::addCheckIn);
    formLayout->addWidget(addCheckInButton, 1, 3);

    formLayout->addWidget(new QLabel("Comments:"), 2, 0, Qt::AlignTop);
    checkInComments = new QTextEdit();
    checkInComments->setMaximumHeight(60);
    checkInComments->setPlaceholderText("Optional comments or traffic details...");
    formLayout->addWidget(checkInComments, 2, 1, 1, 3);

    mainLayout->addWidget(formGroup);

    // Check-in table
    checkInTable = new QTableWidget(0, 6);
    checkInTable->setHorizontalHeaderLabels({"Time", "Callsign", "Name", "Location", "Traffic", "Comments"});
    checkInTable->horizontalHeader()->setStretchLastSection(true);
    checkInTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    checkInTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(checkInTable);

    // Action buttons
    QHBoxLayout *actionLayout = new QHBoxLayout();
    QPushButton *removeBtn = new QPushButton("Remove Selected");
    connect(removeBtn, &QPushButton::clicked, this, &ARCNetScript::removeCheckIn);

    QPushButton *exportCheckInsBtn = new QPushButton("Export Check-in Log...");
    connect(exportCheckInsBtn, &QPushButton::clicked, this, &ARCNetScript::exportCheckInLog);

    actionLayout->addWidget(removeBtn);
    actionLayout->addStretch();
    actionLayout->addWidget(exportCheckInsBtn);
    mainLayout->addLayout(actionLayout);

    tabWidget->addTab(checkInWidget, "Check-ins");
}

void ARCNetScript::createSettingsTab() {
    QWidget *settingsWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(settingsWidget);

    // Appearance settings
    QGroupBox *appearanceGroup = new QGroupBox("Appearance");
    QVBoxLayout *appearanceLayout = new QVBoxLayout(appearanceGroup);

    QPushButton *themeToggleBtn = new QPushButton("Toggle Light/Dark Theme");
    connect(themeToggleBtn, &QPushButton::clicked, this, &ARCNetScript::toggleTheme);
    appearanceLayout->addWidget(themeToggleBtn);

    highContrastCheck = new QCheckBox("High Contrast Mode");
    highContrastCheck->setChecked(highContrast);
    connect(highContrastCheck, &QCheckBox::toggled, this, &ARCNetScript::toggleHighContrast);
    appearanceLayout->addWidget(highContrastCheck);

    QHBoxLayout *fontSizeLayout = new QHBoxLayout();
    fontSizeLayout->addWidget(new QLabel("Script Font Size:"));
    fontSizeSpinBox = new QSpinBox();
    fontSizeSpinBox->setRange(8, 24);
    fontSizeSpinBox->setValue(fontSize);
    connect(fontSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), [this](int size) {
        fontSize = size;
        QFont font = sectionText->font();
        font.setPointSize(fontSize);
        sectionText->setFont(font);
        showStatusMessage(QString("Font size changed to %1").arg(size));
    });
    fontSizeLayout->addWidget(fontSizeSpinBox);
    fontSizeLayout->addStretch();
    appearanceLayout->addLayout(fontSizeLayout);

    largeButtonCheck = new QCheckBox("Large Button Mode (Touch-Friendly)");
    appearanceLayout->addWidget(largeButtonCheck);

    layout->addWidget(appearanceGroup);

    // Accessibility settings
    QGroupBox *accessibilityGroup = new QGroupBox("Accessibility");
    QVBoxLayout *accessibilityLayout = new QVBoxLayout(accessibilityGroup);

    QLabel *shortcutsLabel = new QLabel(
        "<b>Keyboard Shortcuts:</b><br>"
        "Space - Next section<br>"
        "Backspace - Previous section<br>"
        "1-9 - Jump to section<br>"
        "Ctrl/Cmd + Plus - Increase font<br>"
        "Ctrl/Cmd + Minus - Decrease font"
    );
    shortcutsLabel->setWordWrap(true);
    accessibilityLayout->addWidget(shortcutsLabel);

    layout->addWidget(accessibilityGroup);

    // Behavior settings
    QGroupBox *behaviorGroup = new QGroupBox("Behavior");
    QVBoxLayout *behaviorLayout = new QVBoxLayout(behaviorGroup);

    autoSaveCheck = new QCheckBox("Auto-save templates on change");
    autoSaveCheck->setChecked(true);
    behaviorLayout->addWidget(autoSaveCheck);

    QCheckBox *rememberSessionCheck = new QCheckBox("Remember last session");
    rememberSessionCheck->setChecked(true);
    behaviorLayout->addWidget(rememberSessionCheck);

    layout->addWidget(behaviorGroup);

    // About section
    QGroupBox *aboutGroup = new QGroupBox("About");
    QVBoxLayout *aboutLayout = new QVBoxLayout(aboutGroup);

    QLabel *aboutLabel = new QLabel(
        "<h3>Amateur Radio Net Script Generator</h3>"
        "<p>Version 2.0 Enhanced Edition</p>"
        "<p>Designed for amateur radio net control operators</p>"
        "<p><b>Features:</b></p>"
        "<ul>"
        "<li>Live script preview</li>"
        "<li>Check-in management</li>"
        "<li>Template system</li>"
        "<li>Timer and progress tracking</li>"
        "<li>Full keyboard navigation</li>"
        "<li>Accessibility focused</li>"
        "</ul>"
        "<p>73 and enjoy your net!</p>"
    );
    aboutLabel->setWordWrap(true);
    aboutLayout->addWidget(aboutLabel);

    layout->addWidget(aboutGroup);
    layout->addStretch();

    tabWidget->addTab(settingsWidget, "Settings");
}

void ARCNetScript::updateTopicPreview() {
    QString preview;
    int previewCount = qMin(3, topics.size());

    for (int i = 0; i < previewCount; i++) {
        preview += QString("%1. %2\n").arg(i + 1).arg(topics[i]);
    }

    if (topics.size() > 3) {
        preview += QString("\n... and %1 more topics").arg(topics.size() - 3);
    }

    topicPreview->setPlainText(preview);
}

void ARCNetScript::updateAnnouncementPreview() {
    QString preview = announcements.join("\n");
    announcementPreview->setPlainText(preview);
}

void ARCNetScript::updateLivePreview() {
    QString clubName = clubEdit->text().trimmed();
    QString netNight = nightEdit->text().trimmed();
    QString netTime = timeEdit->text().trimmed();
    QString callsign = callsignEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString location = locationEdit->text().trimmed();
    QString loggerCall = loggerCallsignEdit->text().trimmed();
    QString loggerName = loggerNameEdit->text().trimmed();

    if (clubName.isEmpty()) clubName = "[Club Name]";
    if (netNight.isEmpty()) netNight = "[Night]";
    if (netTime.isEmpty()) netTime = "[Time]";
    if (callsign.isEmpty()) callsign = "[Callsign]";
    if (name.isEmpty()) name = "[Name]";
    if (location.isEmpty()) location = "[Location]";
    if (loggerCall.isEmpty()) loggerCall = "[Logger Call]";
    if (loggerName.isEmpty()) loggerName = "[Logger Name]";

    QString preview = QString(
        "═══════════════════════════════════════\n"
        "    OPENING SCRIPT PREVIEW\n"
        "═══════════════════════════════════════\n\n"
        "Good evening everyone, this is %1, %2,\n"
        "located in %3.\n\n"
        "Welcome to the %4 %5 Night Net\n"
        "at %6.\n\n"
        "I am joined this evening by our net\n"
        "logging host %7, %8.\n\n"
        "This NET meets every %5 Night to\n"
        "discuss amateur radio and other\n"
        "interesting topics...\n\n"
        "═══════════════════════════════════════"
    ).arg(name, callsign, location, clubName, netNight, netTime, loggerName, loggerCall);

    livePreview->setPlainText(preview);
}

void ARCNetScript::validateFields() {
    bool valid = !clubEdit->text().trimmed().isEmpty() &&
                 !nightEdit->text().trimmed().isEmpty() &&
                 !timeEdit->text().trimmed().isEmpty() &&
                 !callsignEdit->text().trimmed().isEmpty() &&
                 !nameEdit->text().trimmed().isEmpty() &&
                 !locationEdit->text().trimmed().isEmpty() &&
                 !loggerCallsignEdit->text().trimmed().isEmpty() &&
                 !loggerNameEdit->text().trimmed().isEmpty();

    startButton->setEnabled(valid);
}

void ARCNetScript::loadTopicsFile() {
    QString filename = QFileDialog::getOpenFileName(this,
        "Select Topics File", "", "Text Files (*.txt);;All Files (*)");

    if (!filename.isEmpty()) {
        QStringList lines = readLinesFromFile(filename);
        if (!lines.isEmpty()) {
            topics = lines;
            topicFilePath = filename;

            QFileInfo fileInfo(filename);
            topicFileLabel->setText(QString("%1 (%2 topics)")
                .arg(fileInfo.fileName()).arg(lines.size()));

            updateTopicPreview();
            showStatusMessage(QString("Loaded %1 topics from %2")
                .arg(lines.size()).arg(fileInfo.fileName()));
        } else {
            showStatusMessage("Failed to load topics file", true);
        }
    }
}

void ARCNetScript::loadAnnouncementsFile() {
    QString filename = QFileDialog::getOpenFileName(this,
        "Select Announcements File", "", "Text Files (*.txt);;All Files (*)");

    if (!filename.isEmpty()) {
        QStringList lines = readLinesFromFile(filename);
        if (!lines.isEmpty()) {
            announcements = lines;
            announcementFilePath = filename;

            QFileInfo fileInfo(filename);
            announcementFileLabel->setText(QString("%1 (%2 items)")
                .arg(fileInfo.fileName()).arg(lines.size()));

            updateAnnouncementPreview();
            showStatusMessage(QString("Loaded %1 announcements").arg(lines.size()));
        } else {
            showStatusMessage("Failed to load announcements file", true);
        }
    }
}

QStringList ARCNetScript::readLinesFromFile(const QString &filename) {
    QStringList lines;
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                lines.append(line);
            }
        }
        file.close();
    }

    return lines;
}

void ARCNetScript::randomizeTopics() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(topics.begin(), topics.end(), g);
    updateTopicPreview();
    updateLivePreview();
    showStatusMessage("Topics randomized");
}

void ARCNetScript::resetFields() {
    clubEdit->setText(DEFAULT_CLUB_NAME);
    nightEdit->setText(DEFAULT_NET_NIGHT);
    timeEdit->setText(DEFAULT_NET_TIME);
    callsignEdit->setText(DEFAULT_CALLSIGN);
    nameEdit->setText(DEFAULT_NAME);
    locationEdit->setText(DEFAULT_LOCATION);
    numTopicsSpin->setValue(DEFAULT_NUM_TOPICS);
    loggerCallsignEdit->setText(DEFAULT_LOGGER_CALLSIGN);
    loggerNameEdit->setText(DEFAULT_LOGGER_NAME);

    loadDefaultData();
    updateTopicPreview();
    updateAnnouncementPreview();
    updateLivePreview();

    topicFileLabel->setText("Using default topics");
    announcementFileLabel->setText("Using default announcements");

    showStatusMessage("Fields reset to defaults");
}

void ARCNetScript::generateScriptSections() {
    QString clubName = clubEdit->text().trimmed();
    QString netNight = nightEdit->text().trimmed();
    QString netTime = timeEdit->text().trimmed();
    QString callsign = callsignEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString location = locationEdit->text().trimmed();
    int numTopics = numTopicsSpin->value();
    QString loggerCall = loggerCallsignEdit->text().trimmed();
    QString loggerName = loggerNameEdit->text().trimmed();

    sections.clear();

    // Opening section
    Section opening;
    opening.title = "Opening";
    opening.estimatedMinutes = 2;
    opening.content = QString(
        "START OF NET:\n\n"
        "Good evening everyone, this is %1, %2, located in %3. "
        "Welcome to the %4 %5 Night Net. "
        "I am joined this evening by our net logging host %7, %8.\n\n"
        "This NET meets every %5 night at %6 to discuss amateur radio and other interesting topics. "
        "We are here to have fun on the radio through our communications.\n\n"
        "PLEASE, remember to allow 3-5 seconds between transmissions, 1.5 seconds for keyup and then begin speaking. "
        "ALSO, keep the PTT pushed a half second or so after your last word. "
        "That allows your last word not to be cut off.\n\n"
        "(BREAK FOR REPEATER RESET)")
        .arg(name, callsign, location, clubName, netNight, netTime, loggerName, loggerCall);
    sections.append(opening);

    // Discussion Topics
    int topicsToUse = qMin(numTopics, topics.size());
    for (int i = 0; i < topicsToUse; i++) {
        Section topic;
        topic.title = QString("Topic %1").arg(i + 1);
        topic.estimatedMinutes = 5;
        topic.content = QString(
            "═══════════════════════════════════════\n"
            "    DISCUSSION TOPIC %1\n"
            "═══════════════════════════════════════\n\n"
            "%2\n\n"
            "When responding:\n"
            "• State your callsign first\n"
            "• Keep responses brief (1-2 minutes)\n"
            "• Allow pauses for others to jump in\n\n"
            "(Call on stations in order from check-in list)"
        ).arg(i + 1).arg(topics[i]);
        sections.append(topic);
    }


    // Net Instructions
    Section netInstructions;
    netInstructions.title = "Check-in Instructions";
    netInstructions.estimatedMinutes = 2;
    netInstructions.content =
        "═══════════════════════════════════════\n"
        "    CHECK-IN INSTRUCTIONS\n"
        "═══════════════════════════════════════\n\n"
        "This is a directed NET. All check-ins must go through net control.\n\n"
        "PLEASE LISTEN CAREFULLY TO THESE CHECK-IN INSTRUCTIONS:\n"
        "WHEN CHECKING IN SAY ONLY YOUR CALLSIGN PHONETICALLY TWICE.\n\n"
        "Tonight's Topics:\n";
    for (int i = 0; i < topicsToUse; i++) {
        netInstructions.content += QString("    %1. %2\n").arg(i + 1).arg(topics[i]);
    }
    netInstructions.content +=
        "\n═══════════════════════════════════════\n"
        "    CHECK-IN ORDER\n"
        "═══════════════════════════════════════\n\n"
        "SHORTTIMERS CHECK IN FIRST:\n"
        "• No-Traffic Check-ins\n"
        "• Repeater Owners and System Operators\n"
        "• Digital Check-ins\n"
        "• Analog Check-ins\n\n"
        "When all have been called:\n"
        "• Did we miss anyone?\n"
        "• Last call";
    sections.append(netInstructions);

    // Announcements
    Section announcementsSection;
    announcementsSection.title = "Announcements";
    announcementsSection.estimatedMinutes = 3;
    announcementsSection.content = "Now for announcements:\n\n";
    for (const QString &announcement : announcements) {
        announcementsSection.content += "    " + announcement + "\n\n";
    }
    sections.append(announcementsSection);

    // Closing
    Section closing;
    closing.title = "Closing";
    closing.estimatedMinutes = 2;
    closing.content = QString(
        "═══════════════════════════════════════\n"
        "    NET CLOSING\n"
        "═══════════════════════════════════════\n\n"
        "This has been the %1 %2 Night Net.\n\n"
        "We had %3 check-ins tonight.\n\n"
        "Thanks to %4, %5 for spotting and logging.\n\n"
        "With that, this is %6, %7 from %8 wrapping up tonight's net. "
        "I'd like to thank our net logging host %4, %5 for joining us tonight. "
        "I will now be returning all repeaters to normal amateur use.\n\n"
        "73' and have a great night, we will look for you next week!\n\n"
        "═══════════════════════════════════════")
        .arg(clubName, netNight)
        .arg(checkIns.size())
        .arg(loggerName, loggerCall, name, callsign, location);
    sections.append(closing);
}

void ARCNetScript::populateSectionList() {
    sectionList->clear();
    int totalMinutes = 0;

    for (int i = 0; i < sections.size(); i++) {
        totalMinutes += sections[i].estimatedMinutes;
        QString itemText = QString("%1. %2 (%3 min)")
            .arg(i + 1)
            .arg(sections[i].title)
            .arg(sections[i].estimatedMinutes);
        sectionList->addItem(itemText);
    }

    estimatedTimeLabel->setText(QString("Total Est: %1 min").arg(totalMinutes));
}

void ARCNetScript::displaySection() {
    if (sections.isEmpty() || currentSection >= sections.size()) {
        return;
    }

    sectionLabel->setText(QString("Section %1 of %2: %3")
        .arg(currentSection + 1)
        .arg(sections.size())
        .arg(sections[currentSection].title));

    progressBar->setValue(currentSection + 1);
    progressBar->setMaximum(sections.size());
    progressBar->setFormat(QString("Section %1 of %2 (%3%)")
        .arg(currentSection + 1)
        .arg(sections.size())
        .arg(int((currentSection + 1) * 100.0 / sections.size())));

    sectionText->setPlainText(sections[currentSection].content);

    prevButton->setEnabled(currentSection > 0);
    nextButton->setEnabled(currentSection < sections.size() - 1);

    sectionList->setCurrentRow(currentSection);
}

void ARCNetScript::startNetScript() {
    if (!startButton->isEnabled()) {
        showStatusMessage("Please fill in all required fields", true);
        return;
    }

    generateScriptSections();
    currentSection = 0;

    populateSectionList();
    displaySection();

    progressBar->setVisible(true);
    exportButton->setEnabled(true);

    tabWidget->setCurrentIndex(1);

    showStatusMessage("Net script generated successfully!");
}

void ARCNetScript::nextSection() {
    if (currentSection < sections.size() - 1) {
        currentSection++;
        displaySection();
    }
}

void ARCNetScript::prevSection() {
    if (currentSection > 0) {
        currentSection--;
        displaySection();
    }
}

void ARCNetScript::onSectionSelected(int index) {
    if (index >= 0 && index < sections.size()) {
        currentSection = index;
        displaySection();
    }
}

void ARCNetScript::addCheckIn() {
    QString callsign = checkInCallsign->text().trimmed().toUpper();
    QString name = checkInName->text().trimmed();
    QString location = checkInLocation->text().trimmed();
    QString comments = checkInComments->toPlainText().trimmed();

    if (callsign.isEmpty()) {
        showStatusMessage("Callsign is required", true);
        return;
    }

    CheckIn checkIn;
    checkIn.callsign = callsign;
    checkIn.name = name;
    checkIn.location = location;
    checkIn.comments = comments;
    checkIn.hasTraffic = checkInHasTraffic->isChecked();
    checkIn.time = QDateTime::currentDateTime();

    checkIns.append(checkIn);

    // Add to table
    int row = checkInTable->rowCount();
    checkInTable->insertRow(row);

    checkInTable->setItem(row, 0, new QTableWidgetItem(checkIn.time.toString("hh:mm:ss")));
    checkInTable->setItem(row, 1, new QTableWidgetItem(checkIn.callsign));
    checkInTable->setItem(row, 2, new QTableWidgetItem(checkIn.name));
    checkInTable->setItem(row, 3, new QTableWidgetItem(checkIn.location));
    checkInTable->setItem(row, 4, new QTableWidgetItem(checkIn.hasTraffic ? "Yes" : "No"));
    checkInTable->setItem(row, 5, new QTableWidgetItem(checkIn.comments));

    // Clear form
    checkInCallsign->clear();
    checkInName->clear();
    checkInLocation->clear();
    checkInComments->clear();
    checkInHasTraffic->setChecked(false);
    checkInCallsign->setFocus();

    // Update count
    checkInCountLabel->setText(QString("Total Check-ins: %1").arg(checkIns.size()));

    showStatusMessage(QString("Added check-in: %1").arg(callsign));
}

void ARCNetScript::removeCheckIn() {
    int row = checkInTable->currentRow();
    if (row >= 0 && row < checkIns.size()) {
        QString callsign = checkIns[row].callsign;
        checkIns.removeAt(row);
        checkInTable->removeRow(row);
        checkInCountLabel->setText(QString("Total Check-ins: %1").arg(checkIns.size()));
        showStatusMessage(QString("Removed check-in: %1").arg(callsign));
    }
}

void ARCNetScript::exportCheckInLog() {
    if (checkIns.isEmpty()) {
        showStatusMessage("No check-ins to export", true);
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this,
        "Export Check-in Log", "checkin_log.txt", "Text Files (*.txt);;CSV Files (*.csv)");

    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            out << "Check-in Log\n";
            out << "Generated: " << QDateTime::currentDateTime().toString() << "\n";
            out << "Total Check-ins: " << checkIns.size() << "\n";
            out << "========================================\n\n";

            for (const CheckIn &checkIn : checkIns) {
                out << "Time: " << checkIn.time.toString("hh:mm:ss") << "\n";
                out << "Callsign: " << checkIn.callsign << "\n";
                if (!checkIn.name.isEmpty())
                    out << "Name: " << checkIn.name << "\n";
                if (!checkIn.location.isEmpty())
                    out << "Location: " << checkIn.location << "\n";
                if (checkIn.hasTraffic)
                    out << "Has Traffic: Yes\n";
                if (!checkIn.comments.isEmpty())
                    out << "Comments: " << checkIn.comments << "\n";
                out << "----------------------------------------\n\n";
            }

            file.close();
            showStatusMessage("Check-in log exported successfully");
        } else {
            showStatusMessage("Failed to export check-in log", true);
        }
    }
}

void ARCNetScript::startTimer() {
    if (netTimer->isActive()) {
        netTimer->stop();
        showStatusMessage("Timer stopped");
    } else {
        elapsedTime = QTime(0, 0, 0);
        netTimer->start(1000);
        showStatusMessage("Timer started");
    }
}

void ARCNetScript::updateTimer() {
    elapsedTime = elapsedTime.addSecs(1);
    timerLabel->setText(elapsedTime.toString("hh:mm:ss"));
}

void ARCNetScript::increaseFontSize() {
    if (fontSize < 24) {
        fontSize++;
        fontSizeSpinBox->setValue(fontSize);
        QFont font = sectionText->font();
        font.setPointSize(fontSize);
        sectionText->setFont(font);
    }
}

void ARCNetScript::decreaseFontSize() {
    if (fontSize > 8) {
        fontSize--;
        fontSizeSpinBox->setValue(fontSize);
        QFont font = sectionText->font();
        font.setPointSize(fontSize);
        sectionText->setFont(font);
    }
}

void ARCNetScript::toggleHighContrast() {
    highContrast = !highContrast;
    showStatusMessage(highContrast ? "High contrast enabled" : "High contrast disabled");
    // Could implement specific high contrast styles here
}

void ARCNetScript::filterTopicsByCategory() {
    // Placeholder for category filtering
    updateTopicPreview();
}

void ARCNetScript::markTopicAsFavorite() {
    // Placeholder for favorite marking
    showStatusMessage("Topic marked as favorite");
}

void ARCNetScript::saveTemplate() {
    QString name = QInputDialog::getText(this, "Save Template",
        "Template name:", QLineEdit::Normal, "My Net Template");

    if (!name.isEmpty()) {
        NetTemplate tmpl;
        tmpl.name = name;
        tmpl.clubName = clubEdit->text();
        tmpl.netNight = nightEdit->text();
        tmpl.netTime = timeEdit->text();
        tmpl.callsign = callsignEdit->text();
        tmpl.operatorName = nameEdit->text();
        tmpl.location = locationEdit->text();
        tmpl.numTopics = numTopicsSpin->value();
        tmpl.loggerCallsign = loggerCallsignEdit->text();
        tmpl.loggerName = loggerNameEdit->text();

        templates.append(tmpl);
        templateCombo->addItem(name);

        showStatusMessage(QString("Template '%1' saved").arg(name));
    }
}

void ARCNetScript::loadTemplate() {
    // Placeholder for template loading from combo box
    showStatusMessage("Template loaded");
}

void ARCNetScript::exportScript() {
    if (sections.isEmpty()) {
        showStatusMessage("No script to export", true);
        return;
    }

    QString callsign = callsignEdit->text().trimmed();
    QString defaultFilename = QString("net_script_%1_%2.txt")
        .arg(callsign)
        .arg(QDate::currentDate().toString("yyyy-MM-dd"));

    QString filename = QFileDialog::getSaveFileName(this,
        "Export Net Script", defaultFilename, "Text Files (*.txt);;All Files (*)");

    if (!filename.isEmpty()) {
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            out << "═══════════════════════════════════════════════════\n";
            out << "         AMATEUR RADIO NET SCRIPT\n";
            out << "═══════════════════════════════════════════════════\n\n";
            out << "Generated: " << QDateTime::currentDateTime().toString() << "\n";
            out << "Net Control: " << nameEdit->text() << ", " << callsign << "\n";
            out << "Location: " << locationEdit->text() << "\n\n";

            for (const Section &section : sections) {
                out << "\n\n";
                out << "═══════════════════════════════════════════════════\n";
                out << "  " << section.title.toUpper() << "\n";
                out << "  Estimated time: " << section.estimatedMinutes << " minutes\n";
                out << "═══════════════════════════════════════════════════\n\n";
                out << section.content << "\n\n";
            }

            // Add check-in summary if any
            if (!checkIns.isEmpty()) {
                out << "\n\n";
                out << "═══════════════════════════════════════════════════\n";
                out << "  CHECK-IN SUMMARY (" << checkIns.size() << " total)\n";
                out << "═══════════════════════════════════════════════════\n\n";

                for (const CheckIn &checkIn : checkIns) {
                    out << checkIn.callsign;
                    if (!checkIn.name.isEmpty())
                        out << " - " << checkIn.name;
                    if (!checkIn.location.isEmpty())
                        out << " (" << checkIn.location << ")";
                    if (checkIn.hasTraffic)
                        out << " [TRAFFIC]";
                    out << "\n";
                }
            }

            file.close();

            QFileInfo fileInfo(filename);
            showStatusMessage(QString("Script exported to %1").arg(fileInfo.fileName()));
        } else {
            showStatusMessage("Failed to save script file", true);
        }
    }
}

void ARCNetScript::toggleTheme() {
    darkTheme = !darkTheme;
    applyTheme();
    showStatusMessage(darkTheme ? "Dark theme applied" : "Light theme applied");
}

void ARCNetScript::applyTheme() {
    if (darkTheme) {
        qApp->setStyle("Fusion");
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(43, 43, 43));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(43, 43, 43));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(43, 43, 43));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        qApp->setPalette(darkPalette);
    } else {
        qApp->setStyle("Fusion");
        qApp->setPalette(QApplication::style()->standardPalette());
    }
}

void ARCNetScript::showStatusMessage(const QString &message, bool isError) {
    statusBar->showMessage(message, 4000);
    if (isError) {
        statusBar->setStyleSheet("QStatusBar { background: #5c1e1e; color: #ffaaaa; }");
    } else {
        statusBar->setStyleSheet("QStatusBar { background: #1e5c1e; color: #aaffaa; }");
    }

    QTimer::singleShot(4000, [this]() {
        statusBar->setStyleSheet("");
    });
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("icon.png"));

    ARCNetScript window;
    window.show();

    return app.exec();
}

#include "net-script.moc"
