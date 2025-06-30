#!/usr/bin/python3
import sys
import os
import random
import json
import configparser
from pathlib import Path
from datetime import datetime

from PyQt6.QtWidgets import (
    QApplication, QWidget, QLabel, QLineEdit, QTextEdit, QVBoxLayout, QHBoxLayout,
    QPushButton, QFileDialog, QMessageBox, QSpinBox, QGroupBox, QScrollArea,
    QProgressBar, QListWidget, QSplitter, QCompleter, QFontDialog, QTabWidget,
    QGridLayout, QFrame, QSizePolicy, QToolTip, QCheckBox, QComboBox
)
from PyQt6.QtCore import Qt, QTimer, QSettings, pyqtSignal, QPropertyAnimation, QRect, QEasingCurve
from PyQt6.QtGui import QFont, QIcon, QPalette, QColor, QPixmap, QPainter

DEFAULT_CALLSIGN = "N0CALL"
DEFAULT_NAME = "Net Control"
DEFAULT_LOCATION = "Anytown, USA"
DEFAULT_NUM_TOPICS = 1

class NetConfig:
    def __init__(self):
        self.config_file = 'net_config.ini'
        self.templates_dir = Path('templates')
        self.config = configparser.ConfigParser()
        self.load_or_create_config()
    
    def load_or_create_config(self):
        """Load existing config or create default one"""
        if not os.path.exists(self.config_file):
            self.create_default_config()
        
        self.config.read(self.config_file)
    
    def create_default_config(self):
        """Create default configuration file"""
        config = configparser.ConfigParser()
        
        # Club Information
        config['CLUB'] = {
            'name': 'Amateur Radio Club',
            'net_name': 'Weekly Net',
            'meeting_day': 'Monday',
            'meeting_time': '7:00 PM',
            'timezone': 'Local Time',
            'website': '',
            'email': '',
            'repeater_info': 'Local Repeater'
        }
        
        # Default Operator Information
        config['DEFAULTS'] = {
            'callsign': os.environ.get('HAM_CALLSIGN', 'N0CALL'),
            'name': os.environ.get('HAM_NAME', 'Net Control'),
            'location': os.environ.get('HAM_LOCATION', 'Anytown, USA'),
            'num_topics': '1'
        }
        
        # Script Sections Configuration
        config['SCRIPT_SECTIONS'] = {
            'use_opening': 'true',
            'use_club_announcements': 'true',
            'use_topics': 'true',
            'use_checkins': 'true',
            'use_emergency_traffic': 'false',
            'use_formal_traffic': 'false',
            'use_elmering': 'false',
            'use_roundtable': 'true',
            'use_closing': 'true'
        }
        
        # Custom Text Sections
        config['CUSTOM_TEXT'] = {
            'opening_addition': '',
            'special_instructions': '',
            'checkin_instructions': 'Please say your callsign phonetically twice, followed by your name and location.',
            'closing_addition': 'Thank you for participating in tonight\'s net. 73!'
        }
        
        # Net Format Options
        config['NET_FORMAT'] = {
            'is_directed': 'true',
            'use_roundtable': 'true',
            'allow_comments': 'true',
            'emergency_priority': 'true',
            'formal_traffic': 'false'
        }
        
        with open(self.config_file, 'w') as f:
            config.write(f)
        
        print(f"Created default configuration file: {self.config_file}")
    
    def get_club_info(self):
        """Get club information from config"""
        return {
            'name': self.config.get('CLUB', 'name'),
            'net_name': self.config.get('CLUB', 'net_name'),
            'meeting_day': self.config.get('CLUB', 'meeting_day'),
            'meeting_time': self.config.get('CLUB', 'meeting_time'),
            'timezone': self.config.get('CLUB', 'timezone'),
            'website': self.config.get('CLUB', 'website'),
            'email': self.config.get('CLUB', 'email'),
            'repeater_info': self.config.get('CLUB', 'repeater_info')
        }
    
    def get_defaults(self):
        """Get default operator information"""
        return {
            'callsign': self.config.get('DEFAULTS', 'callsign'),
            'name': self.config.get('DEFAULTS', 'name'),
            'location': self.config.get('DEFAULTS', 'location'),
            'num_topics': self.config.getint('DEFAULTS', 'num_topics')
        }
    
    def get_script_sections(self):
        """Get enabled script sections"""
        return {
            'use_opening': self.config.getboolean('SCRIPT_SECTIONS', 'use_opening'),
            'use_club_announcements': self.config.getboolean('SCRIPT_SECTIONS', 'use_club_announcements'),
            'use_topics': self.config.getboolean('SCRIPT_SECTIONS', 'use_topics'),
            'use_checkins': self.config.getboolean('SCRIPT_SECTIONS', 'use_checkins'),
            'use_emergency_traffic': self.config.getboolean('SCRIPT_SECTIONS', 'use_emergency_traffic'),
            'use_formal_traffic': self.config.getboolean('SCRIPT_SECTIONS', 'use_formal_traffic'),
            'use_elmering': self.config.getboolean('SCRIPT_SECTIONS', 'use_elmering'),
            'use_roundtable': self.config.getboolean('SCRIPT_SECTIONS', 'use_roundtable'),
            'use_closing': self.config.getboolean('SCRIPT_SECTIONS', 'use_closing')
        }
    
    def get_custom_text(self):
        """Get custom text additions"""
        return {
            'opening_addition': self.config.get('CUSTOM_TEXT', 'opening_addition'),
            'special_instructions': self.config.get('CUSTOM_TEXT', 'special_instructions'),
            'checkin_instructions': self.config.get('CUSTOM_TEXT', 'checkin_instructions'),
            'closing_addition': self.config.get('CUSTOM_TEXT', 'closing_addition')
        }
    
    def get_net_format(self):
        """Get net format settings"""
        return {
            'is_directed': self.config.getboolean('NET_FORMAT', 'is_directed'),
            'use_roundtable': self.config.getboolean('NET_FORMAT', 'use_roundtable'),
            'allow_comments': self.config.getboolean('NET_FORMAT', 'allow_comments'),
            'emergency_priority': self.config.getboolean('NET_FORMAT', 'emergency_priority'),
            'formal_traffic': self.config.getboolean('NET_FORMAT', 'formal_traffic')
        }
    
    def save_config(self, section, key, value):
        """Save a configuration value"""
        if section not in self.config:
            self.config.add_section(section)
        self.config.set(section, key, str(value))
        
        with open(self.config_file, 'w') as f:
            self.config.write(f)


    def get_lines_from_file(filepath):
        """Read lines from a text file, filtering out empty lines"""
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                lines = [line.strip() for line in f.readlines() if line.strip()]
            return lines
        except Exception as e:
            print(f"Error reading file {filepath}: {e}")
            return []

class AnimatedButton(QPushButton):
    """Custom button with hover animations"""
    def __init__(self, text, parent=None):
        super().__init__(text, parent)
        self.setMinimumHeight(40)
        self.setCursor(Qt.CursorShape.PointingHandCursor)

    def enterEvent(self, event):
        self.setStyleSheet(self.styleSheet() + "background-color: #4a90e2;")
        super().enterEvent(event)

    def leaveEvent(self, event):
        self.setStyleSheet(self.styleSheet().replace("background-color: #4a90e2;", ""))
        super().leaveEvent(event)

class StatusBar(QFrame):
    """Custom status bar with better visual feedback"""
    def __init__(self):
        super().__init__()
        self.setFrameStyle(QFrame.Shape.StyledPanel)
        self.setMaximumHeight(30)
        
        layout = QHBoxLayout()
        self.status_label = QLabel("")
        self.status_icon = QLabel("")
        
        layout.addWidget(self.status_icon)
        layout.addWidget(self.status_label)
        layout.addStretch()
        self.setLayout(layout)
        
    def show_message(self, message, error=False):
        self.status_label.setText(message)
        if error:
            self.setStyleSheet("background-color: #ffebee; color: #c62828; border: 1px solid #ef5350;")
            self.status_icon.setText("⚠")
        else:
            self.setStyleSheet("background-color: #e8f5e8; color: #2e7d32; border: 1px solid #66bb6a;")
            self.status_icon.setText("✓")
        
        QTimer.singleShot(4000, self.clear_message)
        
    def clear_message(self):
        self.status_label.setText("")
        self.status_icon.setText("")
        self.setStyleSheet("")

class NetControlWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Net Control Script Manager")
        self.setMinimumSize(1200, 800)
        self.resize(1400, 900)
        self.section_idx = 0
        self.sections = []
        self.export_lines = []
        self.topic_file_path = ""
        self.nco_file_path = ""
        self.settings = QSettings("NetControl", "NetApp")
        self.last_font = None
        self.theme_dark = True
        self.auto_advance = False
        self.init_ui()
        self.load_settings()
        self.apply_dark_theme()

    def init_ui(self):
        # Main container with tabs
        self.tab_widget = QTabWidget()
        self.tab_widget.setTabPosition(QTabWidget.TabPosition.North)

        # Setup Tab
        self.setup_tab = QWidget()
        self.init_setup_tab()
        self.tab_widget.addTab(self.setup_tab, "📋 Setup")


        # Script Tab
        self.script_tab = QWidget()
        self.init_script_tab()
        self.tab_widget.addTab(self.script_tab, "📻 Net Script")

        # Settings Tab
        self.settings_tab = QWidget()
        self.init_settings_tab()
        self.tab_widget.addTab(self.settings_tab, "⚙️ Settings")

        # Main layout
        main_layout = QVBoxLayout()
        main_layout.setContentsMargins(10, 10, 10, 10)

        # Header
        header = self.create_header()
        main_layout.addWidget(header)

        main_layout.addWidget(self.tab_widget)

        # Status bar
        self.status_bar = StatusBar()
        main_layout.addWidget(self.status_bar)

        self.setLayout(main_layout)

    def set_splitter_top_bottom_ratio(self, splitter, top_ratio=0.25):
        """Set the splitter ratio for top (top_ratio) and bottom (1-top_ratio)"""
        total = splitter.size().height() or 1000  # fallback if not yet shown
        top = int(total * top_ratio)
        bottom = total - top
        splitter.setSizes([top, bottom])

    def create_header(self):
        header = QFrame()
        header.setFrameStyle(QFrame.Shape.StyledPanel)
        header.setMaximumHeight(80)

        layout = QHBoxLayout()

        # Logo/Icon placeholder
        logo = QLabel("📡")
        logo.setStyleSheet("font-size: 48px;")

        # Title and subtitle
        title_layout = QVBoxLayout()
        title = QLabel("Amateur Radio Net Control Manager")
        title.setStyleSheet("font-size: 24px; font-weight: bold; color: #2c3e50;")
        subtitle = QLabel("Amateur Radio Net Script Manager")
        subtitle.setStyleSheet("font-size: 14px; color: #7f8c8d; font-style: italic;")

        title_layout.addWidget(title)
        title_layout.addWidget(subtitle)

        layout.addWidget(logo)
        layout.addLayout(title_layout)
        layout.addStretch()

        header.setLayout(layout)
        return header

    def init_setup_tab(self):

        """Initialize the setup tab with club and operator information"""
        layout = QVBoxLayout()
        layout.setSpacing(20)

        # Club Information Group
        club_group = QGroupBox("🏛️ Club Information")
        club_layout = QGridLayout()
        club_layout.setSpacing(15)

        # Club input fields
        self.club_name_input = QLineEdit()
        self.club_name_input.setPlaceholderText("Enter your club name")
        self.club_name_input.setMinimumHeight(35)

        self.net_name_input = QLineEdit()
        self.net_name_input.setPlaceholderText("e.g., Weekly Check-in Net")
        self.net_name_input.setMinimumHeight(35)

        self.meeting_day_combo = QComboBox()
        self.meeting_day_combo.addItems(['Monday', 'Tuesday', 'Wednesday',
                                       'Thursday', 'Friday', 'Saturday', 'Sunday'])
        self.meeting_day_combo.setMinimumHeight(35)

        self.meeting_time_input = QLineEdit()
        self.meeting_time_input.setPlaceholderText("7:00 PM")
        self.meeting_time_input.setMinimumHeight(35)

        self.timezone_input = QLineEdit()
        self.timezone_input.setPlaceholderText("e.g., Eastern Time, Local Time")
        self.timezone_input.setMinimumHeight(35)

        self.repeater_info_input = QLineEdit()
        self.repeater_info_input.setPlaceholderText("e.g., 146.52 MHz, Local Repeater")
        self.repeater_info_input.setMinimumHeight(35)

        self.website_input = QLineEdit()
        self.website_input.setPlaceholderText("Club website (optional)")
        self.website_input.setMinimumHeight(35)

        club_layout.addWidget(QLabel("Club Name:"), 0, 0)
        club_layout.addWidget(self.club_name_input, 0, 1)
        club_layout.addWidget(QLabel("Net Name:"), 0, 2)
        club_layout.addWidget(self.net_name_input, 0, 3)
        club_layout.addWidget(QLabel("Meeting Day:"), 1, 0)
        club_layout.addWidget(self.meeting_day_combo, 1, 1)
        club_layout.addWidget(QLabel("Time:"), 1, 2)
        club_layout.addWidget(self.meeting_time_input, 1, 3)
        club_layout.addWidget(QLabel("Timezone:"), 2, 0)
        club_layout.addWidget(self.timezone_input, 2, 1)
        club_layout.addWidget(QLabel("Repeater Info:"), 2, 2)
        club_layout.addWidget(self.repeater_info_input, 2, 3)
        club_layout.addWidget(QLabel("Website:"), 3, 0)
        club_layout.addWidget(self.website_input, 3, 1, 1, 3)

        club_group.setLayout(club_layout)

        # Net Control Operator Information Group
        operator_group = QGroupBox("👤 Net Control Operator Information")
        operator_layout = QGridLayout()
        operator_layout.setSpacing(15)

        # Existing operator fields (keep these the same)
        self.callsign_input = QLineEdit()
        self.callsign_input.setPlaceholderText("Enter your callsign (e.g., W1AW)")
        self.callsign_input.setMinimumHeight(35)

        self.name_input = QLineEdit()
        self.name_input.setPlaceholderText("Enter your name")
        self.name_input.setMinimumHeight(35)

        self.location_input = QLineEdit()
        self.location_input.setPlaceholderText("Enter your location (e.g., Hartford, CT)")
        self.location_input.setMinimumHeight(35)

        self.num_topics_input = QSpinBox()
        self.num_topics_input.setMinimum(1)
        self.num_topics_input.setMaximum(10)
        self.num_topics_input.setMinimumHeight(35)
        self.num_topics_input.setSuffix(" topics")

        operator_layout.addWidget(QLabel("Callsign:"), 0, 0)
        operator_layout.addWidget(self.callsign_input, 0, 1)
        operator_layout.addWidget(QLabel("Name:"), 0, 2)
        operator_layout.addWidget(self.name_input, 0, 3)
        operator_layout.addWidget(QLabel("Location:"), 1, 0)
        operator_layout.addWidget(self.location_input, 1, 1)
        operator_layout.addWidget(QLabel("Number of Topics:"), 1, 2)
        operator_layout.addWidget(self.num_topics_input, 1, 3)

        operator_group.setLayout(operator_layout)

        # Net Format Configuration Group
        format_group = QGroupBox("📻 Net Format Configuration")
        format_layout = QVBoxLayout()
        format_layout.setSpacing(10)

        # Net format checkboxes
        format_options_layout = QGridLayout()

        self.directed_net_cb = QCheckBox("Directed Net (Net Control manages check-ins)")
        self.directed_net_cb.setChecked(True)
        self.directed_net_cb.setToolTip("Net control manages all check-ins and discussions")

        self.roundtable_cb = QCheckBox("Include Roundtable Discussion")
        self.roundtable_cb.setChecked(True)
        self.roundtable_cb.setToolTip("Allow open discussion among participants")

        self.emergency_traffic_cb = QCheckBox("Emergency Traffic Priority")
        self.emergency_traffic_cb.setChecked(True)
        self.emergency_traffic_cb.setToolTip("Give priority to emergency communications")

        self.formal_traffic_cb = QCheckBox("Handle Formal Traffic")
        self.formal_traffic_cb.setChecked(False)
        self.formal_traffic_cb.setToolTip("Include formal message traffic handling")

        self.elmering_cb = QCheckBox("Include Elmering Session")
        self.elmering_cb.setChecked(False)
        self.elmering_cb.setToolTip("Include technical discussion/mentoring time")

        self.comments_cb = QCheckBox("Allow General Comments")
        self.comments_cb.setChecked(True)
        self.comments_cb.setToolTip("Allow participants to share general comments")

        format_options_layout.addWidget(self.directed_net_cb, 0, 0)
        format_options_layout.addWidget(self.roundtable_cb, 0, 1)
        format_options_layout.addWidget(self.emergency_traffic_cb, 1, 0)
        format_options_layout.addWidget(self.formal_traffic_cb, 1, 1)
        format_options_layout.addWidget(self.elmering_cb, 2, 0)
        format_options_layout.addWidget(self.comments_cb, 2, 1)

        format_layout.addLayout(format_options_layout)
        format_group.setLayout(format_layout)

        # Content Management Group (keep existing but update labels)
        content_group = QGroupBox("📝 Content Management")
        content_layout = QVBoxLayout()
        content_layout.setSpacing(15)

        # Topics section
        topics_frame = QFrame()
        topics_frame.setFrameStyle(QFrame.Shape.Box)
        topics_layout = QVBoxLayout()

        topics_header = QHBoxLayout()
        topics_label = QLabel("Discussion Topics")
        topics_label.setStyleSheet("font-weight: bold; font-size: 11px;")

        topic_file_btn = AnimatedButton("📁 Load Topics File")
        topic_file_btn.clicked.connect(self.load_topics_file)
        topic_file_btn.setMinimumSize(120, 32)

        randomize_btn = AnimatedButton("🎲 Randomize")
        randomize_btn.clicked.connect(self.randomize_topics)
        randomize_btn.setMinimumSize(115, 32)

        topics_header.addWidget(topics_label)
        topics_header.addStretch()
        topics_header.addWidget(randomize_btn)
        topics_header.addWidget(topic_file_btn)


        # Use generic topics instead of SkyHubLink-specific ones
        self.topics = self.load_default_topics()
        self.topic_preview = QTextEdit()
        self.topic_preview.setReadOnly(True)
        self.topic_preview.setMaximumHeight(100)
        self.topic_preview.setPlainText("\n".join(self.topics[:3]) + f"\n... and {len(self.topics)-3} more")

        topics_layout.addLayout(topics_header)
        topics_layout.addWidget(self.topic_preview)
        topics_frame.setLayout(topics_layout)

        # Announcements section
        announcements_frame = QFrame()
        announcements_frame.setFrameStyle(QFrame.Shape.Box)
        announcements_layout = QVBoxLayout()

        announcements_header = QHBoxLayout()
        announcements_label = QLabel("Club Announcements")
        announcements_label.setStyleSheet("font-weight: bold; font-size: 14px;")


        announce_file_btn = AnimatedButton("📁 Load Announcements File")
        announce_file_btn.clicked.connect(self.load_announcements_file)  # Updated method name
        announce_file_btn.setMinimumSize(140, 32)

        announcements_header.addStretch()
        announcements_header.addWidget(announce_file_btn)

        # Use generic announcements
        self.club_announcements = self.load_default_announcements()
        self.announce_preview = QTextEdit()
        self.announce_preview.setReadOnly(True)
        self.announce_preview.setMaximumHeight(80)
        self.announce_preview.setPlainText("\n".join(self.club_announcements))

        announcements_layout.addLayout(announcements_header)
        announcements_layout.addWidget(self.announce_preview)
        announcements_frame.setLayout(announcements_layout)

        content_layout.addWidget(topics_frame)
        content_layout.addWidget(announcements_frame)
        content_group.setLayout(content_layout)

        # Action buttons
        action_layout = QHBoxLayout()
        action_layout.setSpacing(15)

        load_template_btn = AnimatedButton("📋 Load Template")
        load_template_btn.clicked.connect(self.load_template)
        load_template_btn.setToolTip("Load a pre-configured net template")
        load_template_btn.setMinimumSize(115, 32)

        save_template_btn = AnimatedButton("💾 Save Template")
        save_template_btn.clicked.connect(self.save_template)
        save_template_btn.setToolTip("Save current settings as a template")
        save_template_btn.setMinimumSize(115, 32)

        self.quit_btn = AnimatedButton("🚪 Quit Application")
        self.quit_btn.clicked.connect(self.quit_net)
        self.quit_btn.setStyleSheet("background-color: #e74c3c; color: white;")

        self.start_btn = AnimatedButton("🚀 Generate Net Script")
        self.start_btn.setMinimumHeight(50)
        self.start_btn.setStyleSheet("font-size: 16px; font-weight: bold; background-color: #27ae60; color: white;")
        self.start_btn.clicked.connect(self.start_net_script)

        reset_btn = AnimatedButton("🔄 Reset Fields")
        reset_btn.clicked.connect(self.reset_fields)
        reset_btn.setMinimumSize(115, 32)

        action_layout.addWidget(load_template_btn)
        action_layout.addWidget(save_template_btn)
        action_layout.addWidget(reset_btn)
        action_layout.addStretch()
        action_layout.addWidget(self.quit_btn)
        action_layout.addWidget(self.start_btn)

        layout.addWidget(club_group)
        layout.addWidget(operator_group)
        layout.addWidget(format_group)
        layout.addWidget(content_group)
        layout.addLayout(action_layout)
        layout.addStretch()

        self.setup_tab.setLayout(layout)

        # Enhanced validation for all fields
        self.callsign_input.textChanged.connect(self.validate_fields)
        self.name_input.textChanged.connect(self.validate_fields)
        self.location_input.textChanged.connect(self.validate_fields)
        self.club_name_input.textChanged.connect(self.validate_fields)
        self.net_name_input.textChanged.connect(self.validate_fields)
        self.validate_fields()

    def load_default_topics(self):
        """Load default discussion topics"""
        return [
            "What's your favorite amateur radio operating mode and why?",
            "Share a memorable QSO or contest experience from your amateur radio journey.",
            "What new amateur radio equipment or technology have you tried recently?",
            "Discuss your favorite amateur radio frequency band and what makes it special.",
            "What amateur radio emergency preparedness activities have you participated in?",
            "Share tips for new amateur radio operators getting started in the hobby.",
            "What amateur radio project are you currently working on or planning?",
            "Discuss the role of amateur radio in your local community.",
            "What's the most interesting amateur radio contact you've made?",
            "Share your thoughts on the future of amateur radio technology.",
            "What amateur radio training or education have you found most valuable?",
            "Discuss your experience with amateur radio public service events.",
            "What advice would you give to someone considering getting their amateur radio license?",
            "Share your favorite amateur radio memory or story.",
            "What role does amateur radio play in your emergency preparedness plans?"
        ]

    def load_default_announcements(self):
        """Load default club announcements"""
        return [
            "Remember to check our club website for the latest news and updates.",
            "Our next club meeting will be announced on the website and mailing list.",
            "New members are always welcome - visit our website for membership information.",
            "Check out our club's social media presence for daily updates and photos.",
            "Upcoming amateur radio events and contests will be posted on our website.",
            "Technical help and elmering sessions are available - contact club officers.",
            "Remember to follow good amateur radio operating practices on all repeaters."
        ]

    # Updated validation method
    def validate_fields(self):
        """Enhanced field validation for club and operator information"""
        operator_valid = all([
            self.callsign_input.text().strip(),
            self.name_input.text().strip(),
            self.location_input.text().strip()
        ])

        club_valid = all([
            self.club_name_input.text().strip(),
            self.net_name_input.text().strip(),
            self.meeting_time_input.text().strip()
        ])

        # Visual feedback for invalid fields
        required_fields = [
            (self.callsign_input, self.callsign_input.text().strip()),
            (self.name_input, self.name_input.text().strip()),
            (self.location_input, self.location_input.text().strip()),
            (self.club_name_input, self.club_name_input.text().strip()),
            (self.net_name_input, self.net_name_input.text().strip()),
            (self.meeting_time_input, self.meeting_time_input.text().strip())
        ]

        for field, value in required_fields:
            if not value:
                field.setStyleSheet("border-color: #e74c3c;")
            else:
                field.setStyleSheet("border-color: #28a745;")

        valid = operator_valid and club_valid
        self.start_btn.setEnabled(valid)
        return valid

    def load_announcements_file(self):
        """Load club announcements from file"""
        file, _ = QFileDialog.getOpenFileName(
            self,
            "Select Announcements File",
            self.announce_file_path if hasattr(self, 'announce_file_path') else os.path.expanduser("~"),
            "Text Files (*.txt);;All Files (*)"
        )
        if file:
            self.announce_file_path = os.path.dirname(file)
            announcements = get_lines_from_file(file)
            if announcements:
                self.club_announcements = announcements  # ✅ Correct assignment
                self.announce_file_label.setText(f"📁 {os.path.basename(file)} ({len(announcements)} items)")
                self.announce_preview.setPlainText("\n".join(self.club_announcements))
                self.status_bar.show_message(f"Loaded {len(announcements)} announcements")
            else:
                QMessageBox.warning(self, "No Announcements Found", "The selected file is empty or unreadable.")
                self.status_bar.show_message("Failed to load announcements file", error=True)

    def init_script_tab(self):
        """Initialize the script execution tab"""
        layout = QVBoxLayout()
        layout.setSpacing(15)

        # Script control section
        control_frame = QFrame()
        control_frame.setFrameStyle(QFrame.Shape.StyledPanel)
        control_frame.setMaximumHeight(60)
        control_layout = QHBoxLayout()


        # Navigation buttons
        self.prev_btn = AnimatedButton("⬅ Previous")
        self.prev_btn.clicked.connect(self.prev_section)
        self.prev_btn.setEnabled(False)
        self.prev_btn.setMinimumSize(115, 32)

        self.next_btn = AnimatedButton("➡ Next")
        self.next_btn.clicked.connect(self.next_section)
        self.next_btn.setEnabled(False)
        self.next_btn.setMinimumSize(115, 32)

        # Section jump controls
        jump_label = QLabel("Jump to Section:")
        self.jump_spinner = QSpinBox()
        self.jump_spinner.setMinimum(1)
        self.jump_spinner.setMaximum(1)
        self.jump_spinner.valueChanged.connect(self.jump_to_section_number)

        # Progress bar
        self.progress = QProgressBar()
        self.progress.setVisible(False)
        self.progress.setTextVisible(True)
        self.progress.setFormat("Section %v of %m")

        # Control buttons
        self.edit_btn = QPushButton("✏ Edit Section")
        self.edit_btn.setCheckable(True)
        self.edit_btn.toggled.connect(self.toggle_section_editing)
        self.edit_btn.setMinimumSize(115, 32)

        self.export_btn = AnimatedButton("💾 Export Script")
        self.export_btn.clicked.connect(self.export_script)
        self.export_btn.setEnabled(False)
        self.export_btn.setMinimumSize(115, 32)

        auto_advance_cb = QCheckBox("Auto-advance (15s)")
        auto_advance_cb.toggled.connect(self.toggle_auto_advance)

        control_layout.addWidget(self.prev_btn)
        control_layout.addWidget(self.next_btn)
        control_layout.addWidget(QLabel("|"))
        control_layout.addWidget(jump_label)
        control_layout.addWidget(self.jump_spinner)
        control_layout.addWidget(QLabel("|"))
        control_layout.addWidget(self.edit_btn)
        control_layout.addWidget(auto_advance_cb)
        control_layout.addStretch()
        control_layout.addWidget(self.export_btn)

        control_frame.setLayout(control_layout)

        # Main content area with splitter
        splitter = QSplitter(Qt.Orientation.Horizontal)

        # Section list (left panel)
        left_panel = QFrame()
        left_panel.setMaximumWidth(200)
        left_layout = QVBoxLayout()

        list_label = QLabel("📋 Script Sections")
        list_label.setStyleSheet("font-weight: bold; font-size: 14px;")

        self.section_list = QListWidget()
        self.section_list.itemClicked.connect(self.jump_to_section)
        self.section_list.setAlternatingRowColors(True)

        left_layout.addWidget(list_label)
        left_layout.addWidget(self.section_list)
        left_panel.setLayout(left_layout)

        # Script content (right panel)
        right_panel = QFrame()
        right_layout = QVBoxLayout()

        # Section header
        self.section_label = QLabel("No script loaded")
        self.section_label.setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; padding: 10px;")

        # Script text area
        self.section_text = QTextEdit()
        self.section_text.setReadOnly(True)
        self.section_text.setFont(QFont("Consolas", 12))
        self.section_text.setPlainText("Generate a net script from the Setup tab to begin.")

        right_layout.addWidget(self.section_label)
        right_layout.addWidget(self.section_text)
        right_panel.setLayout(right_layout)

        # Add panels to splitter
        splitter.addWidget(left_panel)
        splitter.addWidget(right_panel)
        splitter.setSizes([200, 900])  # Give more space to script content
        self.set_splitter_top_bottom_ratio(splitter, 0.25)

        quit_btn = AnimatedButton("🚪 Quit Application")
        quit_btn.clicked.connect(self.quit_net)
        quit_btn.setStyleSheet("background-color: #e74c3c; color: white;")

        # Progress bar at bottom
        progress_layout = QHBoxLayout()
        progress_layout.addWidget(QLabel("Progress:"))
        progress_layout.addWidget(self.progress)
        progress_layout.addStretch()
        progress_layout.addWidget(quit_btn)

        # Main layout assembly
        layout.addWidget(control_frame)
        layout.addWidget(splitter)
        layout.addLayout(progress_layout)

        self.script_tab.setLayout(layout)


    def load_template(self):
        """Load a net configuration template"""
        file, _ = QFileDialog.getOpenFileName(
            self, "Load Net Template", "", "INI Files (*.ini);;All Files (*)"
        )
        if file:
            try:
                # Load template configuration
                temp_config = configparser.ConfigParser()
                temp_config.read(file)

                # Apply template values to form fields
                if 'CLUB' in temp_config:
                    self.club_name_input.setText(temp_config.get('CLUB', 'name', fallback=''))
                    self.net_name_input.setText(temp_config.get('CLUB', 'net_name', fallback=''))
                    self.meeting_time_input.setText(temp_config.get('CLUB', 'meeting_time', fallback=''))
                    self.timezone_input.setText(temp_config.get('CLUB', 'timezone', fallback=''))
                    self.repeater_info_input.setText(temp_config.get('CLUB', 'repeater_info', fallback=''))
                    self.website_input.setText(temp_config.get('CLUB', 'website', fallback=''))

                    # Set meeting day
                    meeting_day = temp_config.get('CLUB', 'meeting_day', fallback='Monday')
                    index = self.meeting_day_combo.findText(meeting_day)
                    if index >= 0:
                        self.meeting_day_combo.setCurrentIndex(index)

                if 'DEFAULTS' in temp_config:
                    self.callsign_input.setText(temp_config.get('DEFAULTS', 'callsign', fallback=''))
                    self.name_input.setText(temp_config.get('DEFAULTS', 'name', fallback=''))
                    self.location_input.setText(temp_config.get('DEFAULTS', 'location', fallback=''))
                    self.num_topics_input.setValue(temp_config.getint('DEFAULTS', 'num_topics', fallback=1))

                if 'NET_FORMAT' in temp_config:
                    self.directed_net_cb.setChecked(temp_config.getboolean('NET_FORMAT', 'is_directed', fallback=True))
                    self.roundtable_cb.setChecked(temp_config.getboolean('NET_FORMAT', 'use_roundtable', fallback=True))
                    self.emergency_traffic_cb.setChecked(temp_config.getboolean('NET_FORMAT', 'emergency_priority', fallback=True))
                    self.formal_traffic_cb.setChecked(temp_config.getboolean('NET_FORMAT', 'formal_traffic', fallback=False))
                    self.elmering_cb.setChecked(temp_config.getboolean('SCRIPT_SECTIONS', 'use_elmering', fallback=False))
                    self.comments_cb.setChecked(temp_config.getboolean('NET_FORMAT', 'allow_comments', fallback=True))

                self.status_bar.show_message(f"Template loaded: {os.path.basename(file)}")

            except Exception as e:
                QMessageBox.critical(self, "Template Load Error", f"Failed to load template:\n{str(e)}")
                self.status_bar.show_message("Failed to load template", error=True)

    def save_template(self):
        """Save current configuration as a template"""
        file, _ = QFileDialog.getSaveFileName(
            self, "Save Net Template", f"net_template_{self.club_name_input.text().replace(' ', '_')}.ini",
            "INI Files (*.ini);;All Files (*)"
        )
        if file:
            try:
                config = configparser.ConfigParser()

                # Club information
                config['CLUB'] = {
                    'name': self.club_name_input.text(),
                    'net_name': self.net_name_input.text(),
                    'meeting_day': self.meeting_day_combo.currentText(),
                    'meeting_time': self.meeting_time_input.text(),
                    'timezone': self.timezone_input.text(),
                    'website': self.website_input.text(),
                    'repeater_info': self.repeater_info_input.text()
                }

                # Operator defaults
                config['DEFAULTS'] = {
                    'callsign': self.callsign_input.text(),
                    'name': self.name_input.text(),
                    'location': self.location_input.text(),
                    'num_topics': str(self.num_topics_input.value())
                }

                # Net format
                config['NET_FORMAT'] = {
                    'is_directed': str(self.directed_net_cb.isChecked()),
                    'use_roundtable': str(self.roundtable_cb.isChecked()),
                    'allow_comments': str(self.comments_cb.isChecked()),
                    'emergency_priority': str(self.emergency_traffic_cb.isChecked()),
                    'formal_traffic': str(self.formal_traffic_cb.isChecked())
                }

                # Script sections
                config['SCRIPT_SECTIONS'] = {
                    'use_elmering': str(self.elmering_cb.isChecked())
                }

                with open(file, 'w') as f:
                    config.write(f)

                self.status_bar.show_message(f"Template saved: {os.path.basename(file)}")

            except Exception as e:
                QMessageBox.critical(self, "Template Save Error", f"Failed to save template:\n{str(e)}")
                self.status_bar.show_message("Failed to save template", error=True)

    # FIX FOR FIELD VALIDATION - Replace your existing validate_fields method
    def validate_fields(self):
        """Enhanced field validation for club and operator information"""
        operator_valid = all([
            self.callsign_input.text().strip(),
            self.name_input.text().strip(),
            self.location_input.text().strip()
        ])

        club_valid = all([
            self.club_name_input.text().strip(),
            self.net_name_input.text().strip(),
            self.meeting_time_input.text().strip()
        ])

        # Visual feedback for invalid fields
        required_fields = [
            (self.callsign_input, self.callsign_input.text().strip()),
            (self.name_input, self.name_input.text().strip()),
            (self.location_input, self.location_input.text().strip()),
            (self.club_name_input, self.club_name_input.text().strip()),
            (self.net_name_input, self.net_name_input.text().strip()),
            (self.meeting_time_input, self.meeting_time_input.text().strip())
        ]

        for field, value in required_fields:
            if not value:
                field.setStyleSheet("border: 2px solid #e74c3c;")
            else:
                field.setStyleSheet("border: 2px solid #28a745;")

        valid = operator_valid and club_valid
        self.start_btn.setEnabled(valid)
        return valid

    def init_settings_tab(self):
        """Initialize settings tab"""
        layout = QVBoxLayout()
        layout.setSpacing(20)

        # Appearance settings
        appearance_group = QGroupBox("🎨 Appearance")
        appearance_layout = QGridLayout()

        font_btn = AnimatedButton("📝 Choose Font")
        font_btn.clicked.connect(self.choose_font)

        theme_btn = AnimatedButton("🌙 Dark Mode")
        theme_btn.setCheckable(True)
        theme_btn.toggled.connect(self.toggle_theme)
        self.theme_btn = theme_btn

        appearance_layout.addWidget(QLabel("Display Font:"), 0, 0)
        appearance_layout.addWidget(font_btn, 0, 1)
        appearance_layout.addWidget(QLabel("Theme:"), 1, 0)
        appearance_layout.addWidget(theme_btn, 1, 1)

        appearance_group.setLayout(appearance_layout)

        # Behavior settings
        behavior_group = QGroupBox("⚙️ Behavior")
        behavior_layout = QVBoxLayout()

        self.remember_settings_cb = QCheckBox("Remember window position and size")
        self.remember_settings_cb.setChecked(True)

        self.confirm_quit_cb = QCheckBox("Confirm before quitting during active net")
        self.confirm_quit_cb.setChecked(True)

        behavior_layout.addWidget(self.remember_settings_cb)
        behavior_layout.addWidget(self.confirm_quit_cb)

        behavior_group.setLayout(behavior_layout)

        # Help section
        help_group = QGroupBox("❓ Help & Shortcuts")
        help_layout = QVBoxLayout()

        shortcuts_text = QTextEdit()
        shortcuts_text.setReadOnly(True)
        shortcuts_text.setFixedHeight(400)

        shortcuts_text.setPlainText(
            "Keyboard Shortcuts:\n"
            "Ctrl+S: Start Net Script\n"
            "Ctrl+Right: Next Section\n"
            "Ctrl+Left: Previous Section\n"
            "Ctrl+E: Export Script\n"
            "Esc: Quit Application\n\n"
            "Tips:\n"
            "• Use the section list to quickly jump to any part of the script\n"
            "• Enable 'Edit Section' to modify script content on the fly\n"
            "• Load custom topics and announcements from text files\n"
            "• Export your completed script for record keeping"
        )

        help_layout.addWidget(shortcuts_text)
        help_group.setLayout(help_layout)

        # Action buttons
        action_layout = QHBoxLayout()

        help_btn = AnimatedButton("📖 Show Help")
        help_btn.clicked.connect(self.show_shortcuts_help)

        quit_btn = AnimatedButton("🚪 Quit Application")
        quit_btn.clicked.connect(self.quit_net)
        quit_btn.setStyleSheet("background-color: #e74c3c; color: white;")

        action_layout.addWidget(help_btn)
        action_layout.addStretch()
        action_layout.addWidget(quit_btn)

        layout.addWidget(appearance_group)
        layout.addWidget(behavior_group)
        layout.addWidget(help_group)
        layout.addLayout(action_layout)
        layout.addStretch()

        self.settings_tab.setLayout(layout)

    def apply_modern_theme(self):
        """Apply modern styling to the application"""
        style = """
        QWidget {
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 13px;
            color: #212529;
            background-color: #f8f9fa;
        }

        QTabWidget::pane {
            border: 1px solid #c0c0c0;
            color: #212529;
            background-color: #f8f9fa;
        }

        QTabBar::tab {
            background: #e0e0e0;
            border: 1px solid #b0b0b0;
            padding: 8px 16px;
            margin-right: 2px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
        }

        QTabBar::tab:selected {
            background: #d0d0d0;
            border-color: #a0a0a0;
            border-bottom: none;
        }

        QGroupBox {
            font-weight: bold;
            border: 2px solid #cccccc;
            border-radius: 8px;
            margin-top: 10px;
            padding: 10px;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 10px;
            padding: 0 8px;
        }

        QLineEdit, QSpinBox, QComboBox {
            border: 2px solid #ddd;
            border-radius: 4px;
            padding: 6px 8px;
            font-size: 13px;
            color: #212529;
            background-color: #ffffff;
        }

        QLineEdit:focus, QSpinBox:focus, QComboBox:focus {
            border-color: #4a90e2;
            background-color: #f0f8ff;
        }

        QPushButton {
            color: #212529;
            background-color: #17a2b8;
            border: 2px solid #138496;
            border-radius: 6px;
            padding: 8px 16px;
            color: white;
            font-weight: 500;
        }

        QPushButton:hover {
            background-color: #138496;
            border-color: #117a8b;
        }

        QPushButton:pressed {
            background-color: #117a8b;
        }

        QPushButton:disabled {
            background-color: #e0e0e0;
            color: #6c757d;
            border-color: #cccccc;
        }

        QTextEdit {
            border: 1px solid #ccc;
            border-radius: 4px;
            background-color: #f4f4f4;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 14px;
            line-height: 1.4;
        }

        QListWidget {
            border: 1px solid #ccc;
            border-radius: 4px;
            background-color: #ffffff;
            alternate-background-color: #f1f3f5;
        }

        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #e9ecef;
        }

        QListWidget::item:selected {
            background-color: #4a90e2;
            color: white;
        }

        QProgressBar {
            border: 1px solid #ccc;
            border-radius: 4px;
            text-align: center;
            height: 20px;
        }

        QProgressBar::chunk {
            background-color: #28a745;
            border-radius: 4px;
        }

        QLabel {
            font-weight: normal;
        }

        QCheckBox {
            padding: 4px;
        }
        """
        self.setStyleSheet(style)


    def validate_fields(self):
        """Enhanced field validation for club and operator information"""
        operator_valid = all([
            self.callsign_input.text().strip(),
            self.name_input.text().strip(),
            self.location_input.text().strip()
        ])

        club_valid = all([
            self.club_name_input.text().strip(),
            self.net_name_input.text().strip(),
            self.meeting_time_input.text().strip()
        ])

        # Visual feedback for invalid fields
        required_fields = [
            (self.callsign_input, self.callsign_input.text().strip()),
            (self.name_input, self.name_input.text().strip()),
            (self.location_input, self.location_input.text().strip()),
            (self.club_name_input, self.club_name_input.text().strip()),
            (self.net_name_input, self.net_name_input.text().strip()),
            (self.meeting_time_input, self.meeting_time_input.text().strip())
        ]

        for field, value in required_fields:
            if not value:
                field.setStyleSheet("border: 2px solid #e74c3c;")
            else:
                field.setStyleSheet("border: 2px solid #28a745;")

        valid = operator_valid and club_valid
        self.start_btn.setEnabled(valid)
        return valid

    def toggle_auto_advance(self, checked):
        """Toggle auto-advance functionality"""
        self.auto_advance = checked
        if checked:
            self.status_bar.show_message("Auto-advance enabled - sections will advance automatically")
        else:
            self.status_bar.show_message("Auto-advance disabled")

    def jump_to_section_number(self, section_num):
        """Jump to a specific section by number"""
        if 1 <= section_num <= len(self.sections):
            self.section_idx = section_num - 1
            self.display_section()
            self.update_navigation()

    # Keep all the existing methods but with improved status messages
    def load_topics_file(self):
        file, _ = QFileDialog.getOpenFileName(
            self, "Select Topics File", self.topic_file_path or "", "Text Files (*.txt);;All Files (*)"
        )
        if file:
            self.topic_file_path = os.path.dirname(file)
            topics = get_lines_from_file(file)
            if topics:
                self.topics = topics
                self.topic_file_label.setText(f"📁 {os.path.basename(file)} ({len(topics)} topics)")
                self.topic_preview.setPlainText("\n".join(self.topics[:3]) + f"\n... and {len(topics)-3} more")
                self.status_bar.show_message(f"Loaded {len(topics)} topics from {os.path.basename(file)}")
            else:
                QMessageBox.warning(self, "No Topics Found", "The selected file is empty or unreadable.")
                self.status_bar.show_message("Failed to load topics file", error=True)

    def load_nco_file(self):
        file, _ = QFileDialog.getOpenFileName(
            self, "Select Announcements File", self.nco_file_path or "", "Text Files (*.txt);;All Files (*)"
        )
        if file:
            self.nco_file_path = os.path.dirname(file)
            announcements = get_lines_from_file(file)
            if announcements:
                self.nco_announcements = announcements
                self.nco_file_label.setText(f"📁 {os.path.basename(file)} ({len(announcements)} items)")
                self.nco_preview.setPlainText("\n".join(self.nco_announcements))
                self.status_bar.show_message(f"Loaded {len(announcements)} announcements")
            else:
                QMessageBox.warning(self, "No Announcements Found", "The selected file is empty or unreadable.")
                self.status_bar.show_message("Failed to load announcements file", error=True)

    def choose_font(self):
        font, ok = QFontDialog.getFont(self.section_text.font(), self, "Choose Script Font")
        if ok:
            self.section_text.setFont(font)
            self.topic_preview.setFont(font)
            self.nco_preview.setFont(font)
            self.last_font = font
            self.status_bar.show_message("Font updated successfully")

    def toggle_theme(self, dark_mode):
        """Toggle between light and dark themes"""
        self.theme_dark = dark_mode
        if dark_mode:
            self.apply_dark_theme()
            self.status_bar.show_message("Dark theme enabled")
        else:
            self.apply_modern_theme()
            self.status_bar.show_message("Light theme enabled")

    def apply_dark_theme(self):
        """Apply dark theme styling"""
        dark_style = """
        QWidget {
            background-color: #2b2b2b;
            color: #ffffff;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 13px;
        }

        QTabWidget::pane {
            border: 1px solid #555555;
            background-color: #2b2b2b;
        }

        QTabBar::tab {
            background: #3c3c3c;
            border: 1px solid #555555;
            padding: 8px 16px;
            margin-right: 2px;
            color: #ffffff;
        }

        QTabBar::tab:selected {
            background: #2b2b2b;
            border-bottom-color: #2b2b2b;
        }

        QGroupBox {
            font-weight: bold;
            border: 2px solid #555555;
            border-radius: 8px;
            margin-top: 10px;
            padding-top: 10px;
            color: #ffffff;
        }

        QLineEdit, QSpinBox {
            border: 2px solid #555555;
            border-radius: 4px;
            padding: 8px;
            font-size: 13px;
            background-color: #3c3c3c;
            color: #ffffff;
        }

        QLineEdit:focus, QSpinBox:focus {
            border-color: #4a90e2;
        }

        QPushButton {
            background-color: #3c3c3c;
            border: 2px solid #555555;
            border-radius: 4px;
            padding: 4px 8px;
            font-weight: 500;
            color: #ffffff;
        }

        QPushButton:hover {
            background-color: #4a4a4a;
            border-color: #777777;
        }

        QTextEdit {
            border: 1px solid #555555;
            border-radius: 4px;
            background-color: #3c3c3c;
            color: #ffffff;
            font-family: 'Consolas', 'Courier New', monospace;
            font-size: 14px;
            line-height: 1.4;
        }

        QListWidget {
            border: 1px solid #555555;
            border-radius: 4px;
            background-color: #3c3c3c;
            color: #ffffff;
            alternate-background-color: #2b2b2b;
        }

        QListWidget::item:selected {
            background-color: #4a90e2;
            color: white;
        }
        """
        self.setStyleSheet(dark_style)

    def randomize_topics(self):
        """Randomize the order of topics"""
        random.shuffle(self.topics)
        self.topic_preview.setPlainText("\n".join(self.topics[:3]) + f"\n... and {len(self.topics)-3} more")
        self.status_bar.show_message("Topics randomized")

    def reset_fields(self):
        """Reset all input fields to defaults"""
        # Load defaults from config
        config = NetConfig()
        defaults = config.get_defaults()
        club_info = config.get_club_info()
        self.meeting_time_input.setText(club_info['meeting_time'])

        # Reset operator fields
        self.callsign_input.setText(defaults['callsign'])
        self.name_input.setText(defaults['name'])
        self.location_input.setText(defaults['location'])
        self.num_topics_input.setValue(defaults['num_topics'])

        # Reset club fields
        self.club_name_input.setText(club_info['name'])
        self.net_name_input.setText(club_info['net_name'])
        self.meeting_time_input.setText(club_info['meeting_time'])
        self.timezone_input.setText(club_info['timezone'])
        self.repeater_info_input.setText(club_info['repeater_info'])
        self.website_input.setText(club_info['website'])

        # Reset topics and announcements
        self.topics = self.load_default_topics()
        self.club_announcements = self.load_default_announcements()

        # Reset labels and previews
#        self.topic_file_label.setText("Using default topics")
#        self.announce_file_label.setText("Using default announcements")
        self.topic_preview.setPlainText("\n".join(self.topics[:3]) + f"\n... and {len(self.topics)-3} more")
        self.announce_preview.setPlainText("\n".join(self.club_announcements))

        self.status_bar.show_message("Fields reset to defaults")

    def start_net_script(self):
        """Generate and start the net script"""
        if not self.validate_fields():
            self.status_bar.show_message("Please fill in all required fields", error=True)
            return

        self.generate_script_sections()
        self.section_idx = 0
        self.progress.setMaximum(len(self.sections))
        self.progress.setVisible(True)
        self.display_section()
        self.update_navigation()
        self.populate_section_list()

        # Switch to script tab
        self.tab_widget.setCurrentIndex(1)
        self.status_bar.show_message("Net script generated successfully!")

    def generate_script_sections(self):
        """Generate all script sections"""
        callsign = self.callsign_input.text().strip()
        name = self.name_input.text().strip()
        location = self.location_input.text().strip()
        time = self.meeting_time_input.text().strip()
        club_name = self.club_name_input.text().strip()
        day = self.meeting_day_combo.currentText().strip()
        net_name = self.net_name_input.text().strip()
        num_topics = self.num_topics_input.value()

        selected_topics = random.sample(self.topics, min(num_topics, len(self.topics)))

        self.sections = []
        self.export_lines = []

        # Opening
        opening_text = f"""START OF NET:

    Good evening everyone, it is {time} in the Rocky Mountains. This is {name}, {callsign}, located in {location}. Welcome to the {club_name} {day} Night {net_name}.

    This NET meets every {time} Night to discuss amateur radio and other interesting topics. We are here to have fun on the radio through our communications and help to make the use of many fine repeaters that otherwise might be very underutilized and quiet. That also lets others hear that the repeaters are on the air, and that they are active!

    When more than 2 people are using the repeaters, please set up a 'rotation' so that you won't be doubling over the other parties in a 'round table' discussion. It is VERY IMPORTANT to remember you are on many repeaters simultaneously. Good amateur operating practice is remembering to allow all who might want to join in an opportunity to do so, just like using a single stand-alone repeater. Long ragchews are welcome, though remember, most of the repeaters have a 3-minute timeout, keep that in mind as you talk to a friend.

    (BREAK FOR REPEATER RESET)"""


        # Connection Instructions
        connect_text = """    When monitoring the repeater system, we ask that you DO NOT 'KERCHUNCK' the system or repeater to test to see if you are keying up the system. PLEASE DO come on and ask for a radio check. KERCHUNCKING the system can cause link issues. Once again, to see if you are making the 'repeater' actually come on and ask for a radio check. Most of the time you will get an answer from someone. AND, if YOU hear anyone ask for a radio check and no one responds fairly quickly, then you come on and advise the asking station that they are making the system and can be heard, or give a proper signal report.

    (BREAK FOR REPEATER RESET)"""

        # Open Floor
        open_floor_text = """Now we'll open the floor for any additional comments, questions, or announcements. This is your opportunity to share anything ham radio related that we haven't covered tonight. Please give your callsign twice if you have something to share."""

        # Net Instructions
        net_text = f"""    This is a directed NET. All check-ins must go through net control. We will take a few check-ins then do a roundtable of that list, then take another check-in list. We take check-ins by Modes.


    PLEASE LISTEN CAREFULLY TO THESE CHECK-IN INSTRUCTIONS:
        WHEN CHECKING IN SAY ONLY YOUR CALLSIGN PHONETICALLY TWICE. Please to facilitate the system keying up allow a key up time of 1.5 seconds, and hold the PTT a half second or so at the last syllable so that you don't get cut off. If Net control misses you then please stand by for the next round of check-ins.

    Tonight's Topic:\n\n\t{''.join(selected_topics)}

    SHORTTIMERS CHECK IN FIRST.
        No-Traffic Check-ins - Make sure to acknowledge check-ins
        Repeater Owners and System Operators - Make sure to acknowledge check-ins
        Digital Check-ins - Make sure to acknowledge check-ins
        Analog Check-ins - Make sure to acknowledge check-ins

    {open_floor_text}

    When all have been called:
        Did we miss anyone?
        Last call"""

        closing_text = f"""    This has been the {club_name} {day} Night {net_name}. With that, this is {name}, {callsign} from {location} wrapping up tonight's net and returning all systems to normal amateur use. 73' and have a great night, we will look for you next week!"""

        self.sections.append((f"Opening", opening_text))

        self.sections.append(("Connection Instructions", connect_text))

        # Net Control Announcements
        announcements = getattr(self, "nco_announcements", self.club_announcements)
        announcements_text = "Now for Net Control announcements:\n\n    " + "\n\n".join(announcements)
        self.sections.append(("Net Control Announcements", announcements_text))

        # Discussion Topics
        for i, topic in enumerate(selected_topics, 1):
            topic_text = f"Discussion Topic {i}:\n\n    {topic}"
            self.sections.append((f"Topic {i}", topic_text))
        self.sections.append(("Net Instructions", net_text))

        self.sections.append(("Closing", closing_text))

    def display_section(self):
        """Display the current section"""
        if not self.sections or self.section_idx >= len(self.sections):
            return

        title, content = self.sections[self.section_idx]
        self.section_label.setText(f"Section {self.section_idx + 1}: {title}")
        self.section_text.setPlainText(content)
        self.progress.setValue(self.section_idx + 1)

        # Update section list selection
        if self.section_list.count() > 0:
            self.section_list.setCurrentRow(self.section_idx)

        # Auto-advance if enabled
        if self.auto_advance and self.section_idx < len(self.sections) - 1:
            QTimer.singleShot(15000, self.next_section)  # 15 seconds

    def populate_section_list(self):
        """Populate the section navigation list"""
        self.section_list.clear()
        for i, (title, _) in enumerate(self.sections):
            self.section_list.addItem(f"{i+1}. {title}")

        self.jump_spinner.setMaximum(len(self.sections))
        self.jump_spinner.setValue(1)

    def jump_to_section(self, item):
        """Jump to selected section from list"""
        self.section_idx = self.section_list.row(item)
        self.display_section()
        self.update_navigation()

    def next_section(self):
        """Move to next section"""
        if self.section_idx < len(self.sections) - 1:
            self.section_idx += 1
            self.display_section()
            self.update_navigation()

    def prev_section(self):
        """Move to previous section"""
        if self.section_idx > 0:
            self.section_idx -= 1
            self.display_section()
            self.update_navigation()

    def update_navigation(self):
        """Update navigation button states"""
        has_sections = len(self.sections) > 0
        self.prev_btn.setEnabled(has_sections and self.section_idx > 0)
        self.next_btn.setEnabled(has_sections and self.section_idx < len(self.sections) - 1)
        self.export_btn.setEnabled(has_sections)
        self.jump_spinner.setEnabled(has_sections)

    def toggle_section_editing(self, enabled):
        """Toggle editing mode for current section"""
        self.section_text.setReadOnly(not enabled)
        if enabled:
            self.section_text.setStyleSheet("background-color: black; border: 2px solid #ffc107;")
            self.status_bar.show_message("Section editing enabled - changes will be saved automatically")
        else:
            self.section_text.setStyleSheet("")
            # Save changes
            if self.sections and self.section_idx < len(self.sections):
                title = self.sections[self.section_idx][0]
                new_content = self.section_text.toPlainText()
                self.sections[self.section_idx] = (title, new_content)
            self.status_bar.show_message("Section editing disabled - changes saved")

    def export_script(self):
        """Export the complete script to a file"""
        if not self.sections:
            self.status_bar.show_message("No script to export", error=True)
            return

        date_str = datetime.now().strftime("%Y-%m-%d")
        suggested_name = f"net_script_{self.callsign_input.text()}_{date_str}.txt"
        file, _ = QFileDialog.getSaveFileName(
            self, "Export Net Script", suggested_name,
            "Text Files (*.txt);;All Files (*)"
        )

        if file:
            try:
                with open(file, "w") as f:
                    f.write(f"Net Control Script\n")
                    f.write(f"Net Control: {self.callsign_input.text()} - {self.name_input.text()}\n")
                    f.write(f"Location: {self.location_input.text()}\n")
                    f.write("=" * 50 + "\n\n")

                    for i, (title, content) in enumerate(self.sections, 1):
                        f.write(f"SECTION {i}: {title.upper()}\n")
                        f.write("-" * 30 + "\n")
                        f.write(content)
                        f.write("\n\n" + "=" * 50 + "\n\n")

                self.status_bar.show_message(f"Script exported to {os.path.basename(file)}")
            except Exception as e:
                QMessageBox.critical(self, "Export Error", f"Failed to export script:\n{str(e)}")
                self.status_bar.show_message("Export failed", error=True)

    def show_shortcuts_help(self):
        """Show keyboard shortcuts help dialog"""
        QMessageBox.information(
            self, "Keyboard Shortcuts",
            "Ctrl+S: Start Net Script\n"
            "Ctrl+Right: Next Section\n"
            "Ctrl+Left: Previous Section\n"
            "Ctrl+E: Export Script\n"
            "Esc: Quit Application\n\n"
            "Click on section names in the left panel to jump directly to any section.\n"
            "Use the 'Edit Section' button to modify script content on the fly."
        )

    def load_settings(self):
        """Load application settings"""
        # Defensive: only set text if widget still exists
        if hasattr(self, "callsign_input") and self.callsign_input is not None:
            try:
                self.callsign_input.setText(self.settings.value("callsign", DEFAULT_CALLSIGN))
            except RuntimeError:
                print("callsign_input was deleted before settings could be loaded.")
        if hasattr(self, "name_input") and self.name_input is not None:
            try:
                self.name_input.setText(self.settings.value("name", DEFAULT_NAME))
            except RuntimeError:
                print("name_input was deleted before settings could be loaded.")
        if hasattr(self, "location_input") and self.location_input is not None:
            try:
                self.location_input.setText(self.settings.value("location", DEFAULT_LOCATION))
            except RuntimeError:
                print("location_input was deleted before settings could be loaded.")
        if hasattr(self, "num_topics_input") and self.num_topics_input is not None:
            try:
                self.num_topics_input.setValue(int(self.settings.value("num_topics", DEFAULT_NUM_TOPICS)))
            except RuntimeError:
                print("num_topics_input was deleted before settings could be loaded.")

        # Load window geometry
        if self.settings.value("geometry"):
            self.restoreGeometry(self.settings.value("geometry"))

        # Load theme
        self.theme_dark = self.settings.value("dark_theme", False, type=bool)
        self.theme_btn.setChecked(self.theme_dark)
        if self.theme_dark:
            self.apply_dark_theme()

    def save_settings(self):
        """Save application settings"""
        self.settings.setValue("callsign", self.callsign_input.text())
        self.settings.setValue("name", self.name_input.text())
        self.settings.setValue("location", self.location_input.text())
        self.settings.setValue("location", self.location_input.text())
        self.settings.setValue("num_topics", self.num_topics_input.value())
        self.settings.setValue("geometry", self.saveGeometry())
        self.settings.setValue("dark_theme", self.theme_dark)

    def quit_net(self):
        """Quit the application with optional confirmation"""
        if hasattr(self, 'sections') and self.sections and hasattr(self, 'confirm_quit_cb') and self.confirm_quit_cb.isChecked():
            reply = QMessageBox.question(
                self, "Confirm Quit",
                "A net script is currently active. Are you sure you want to quit?",
                QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No,
                QMessageBox.StandardButton.No
            )
            if reply != QMessageBox.StandardButton.Yes:
                return

        self.save_settings()
        QApplication.quit()

    def closeEvent(self, event):
        """Handle application close event"""
        self.save_settings()
        event.accept()

    def keyPressEvent(self, event):
        """Handle keyboard shortcuts"""
        if event.key() == Qt.Key.Key_Escape:
            self.quit_net()
        elif event.modifiers() == Qt.KeyboardModifier.ControlModifier:
            if event.key() == Qt.Key.Key_S:
                self.start_net_script()
            elif event.key() == Qt.Key.Key_Right:
                self.next_section()
            elif event.key() == Qt.Key.Key_Left:
                self.prev_section()
            elif event.key() == Qt.Key.Key_E:
                self.export_script()
        super().keyPressEvent(event)


def main():
    app = QApplication(sys.argv)
    app.setApplicationName("NetControl")
    app.setApplicationVersion("2.0")
    
    window = NetControlWindow()
    window.show()
    
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
