#include"mydialog.hpp"
#include<QDir>
#include<QFileDialog>
#include"errormacroes.hpp"
#include"mydialogcatchevents.hpp"
#include"appclass.hpp"
#include<QTimer>
#include"appwindow.hpp"
#include"config.hpp"
#include"substancefileerrorslist.hpp"
#include"materialschangeswidget.hpp"
#include<QScrollArea>
#include<QScrollBar>

MyDialog::MyDialog(DialogType setType, QWidget *setParent){
    parent = setParent;
    type = setType;
    catchEvent = new MyDialogCatchEvents(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setStyleSheet("MyDialog{border: 2px ridge grey;}");
    createDialog();
    show();
}

MyDialog::MyDialog(DialogType setType, QString title, QString content, bool setYesNoQuestion,QString setFirstOption, QString setSecondOption, QString setThirdOption, QWidget *setParent){
    parent = setParent;
    type = setType;
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setStyleSheet("MyDialog{border: 2px ridge grey;}");
    yesNoQuestion = setYesNoQuestion;
    catchEvent = new MyDialogCatchEvents(this);
    createDialog();
    QFontMetrics titleMetrics(QFont("Comic Sans MS",12, QFont::Medium));
    QFontMetrics contentMetrics(font());
    titleLabel->setWordWrap(true);
    titleLabel->setFont(QFont("Comic Sans MS",12, QFont::Medium));
    questionLabel->setWordWrap(true);
    switch(type){
    case SUBSTANCE_FILE_ERRORS_LIST:
    {
        mcw = static_cast<MaterialsChangesWidget<SubstancesParamsFileParam>*>(parent);
        delete secondOptionLabel;
        secondOptionLabel = nullptr;
        delete thirdOptionLabel;
        thirdOptionLabel = nullptr;
        scrollArea = new QScrollArea(this);
        scrollArea->setStyleSheet("background-color: transparent");
        scrollArea->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{background: transparent; width: 5px;} QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical, QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{border: none; background: none; color: none;} QScrollBar::handle:vertical{background-color: white; border: 1px solid black;}");
        scrollArea->setLayoutDirection(Qt::RightToLeft);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setFixedWidth(SUBSTANCE_FILE_ERROR_LIST_WIDTH + 2 * HORIZONTAL_OFFSET + 7);
        titleLabel->setGeometry(HORIZONTAL_OFFSET, VERTICAL_BORDER_OFFSET, SUBSTANCE_FILE_ERROR_LIST_WIDTH, titleMetrics.boundingRect(0, 0, SUBSTANCE_FILE_ERROR_LIST_WIDTH, MAX_TEST_RECT_HEIGHT, Qt::AlignHCenter | Qt::TextWordWrap, title).height() + 5);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setText(title);
        questionLabel->setAlignment(Qt::AlignCenter);
        questionLabel->setGeometry(HORIZONTAL_OFFSET, titleLabel->y() + titleLabel->height() + VERTICAL_OFFSET, SUBSTANCE_FILE_ERROR_LIST_WIDTH, contentMetrics.boundingRect(0, 0, SUBSTANCE_FILE_ERROR_LIST_WIDTH, MAX_TEST_RECT_HEIGHT, Qt::AlignHCenter | Qt::TextWordWrap, content).height() + 5);
        questionLabel->setText(content);
        scrollArea->setGeometry(HORIZONTAL_OFFSET, questionLabel->y() + questionLabel->height() + VERTICAL_OFFSET, SUBSTANCE_FILE_ERROR_LIST_WIDTH + 7, SUBSTANCE_FILE_ERROR_LIST_HEIGHT + 2);
        substanceFileErrorsList = new SubstanceFileErrorsList(this);
        substanceFileErrorsList->move(HORIZONTAL_OFFSET + 5, questionLabel->y() + questionLabel->height() + VERTICAL_OFFSET);
        firstOptionLabel->setAlignment(Qt::AlignCenter);
        firstOptionLabel->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
        firstOptionLabel->setGeometry((width() - 100) * 0.5, questionLabel->y() + questionLabel->height() + 2 * VERTICAL_OFFSET + SUBSTANCE_FILE_ERROR_LIST_HEIGHT, 100, BUTTONS_HEIGHT);
        firstOptionLabel->setText(QString("Zamknij"));
        firstOptionLabel->installEventFilter(catchEvent);
    }
        break;
    case QUESTION_DIALOG:
    {
        int referenceWidth = 0;
        int numbOfButtons = 0;
        int buttonWidth = 0;
         if(setYesNoQuestion){
            numbOfButtons = 2;
            referenceWidth = YES_NO_BUTTON_WIDTH;
        }else{
            if(!setFirstOption.isEmpty()){
                if(!setSecondOption.isEmpty()){
                    if(!setThirdOption.isEmpty()){
                        numbOfButtons = 3;
                        referenceWidth = contentMetrics.horizontalAdvance(setThirdOption);
                    }else{
                        numbOfButtons = 2;
                    }
                    int tempHA = contentMetrics.horizontalAdvance(setSecondOption);
                    if(referenceWidth < tempHA)
                        referenceWidth = tempHA;
                }else{
                    numbOfButtons = 1;
                }
                int tempHA = contentMetrics.horizontalAdvance(setFirstOption);
                if(referenceWidth < tempHA)
                    referenceWidth = tempHA;
            }
        }
        referenceWidth += 15;
        switch(numbOfButtons){
        case 2:
            referenceWidth += HORIZONTAL_OFFSET + referenceWidth;
            break;
        case 3:
            referenceWidth += 2* HORIZONTAL_OFFSET + 2* referenceWidth;
            break;
        default:
            break;
        }
        if(referenceWidth < MIN_REFERENCE_WIDTH){
            if(yesNoQuestion)
                buttonWidth = YES_NO_BUTTON_WIDTH;
            else
                if((buttonWidth = (referenceWidth - (numbOfButtons - 1) * HORIZONTAL_OFFSET) / numbOfButtons) < 40)
                    buttonWidth = 40;
            referenceWidth = MIN_REFERENCE_WIDTH;
        }
        else{
            buttonWidth = (referenceWidth - (numbOfButtons - 1) * HORIZONTAL_OFFSET) / numbOfButtons;
        }
        setFixedWidth(referenceWidth + 2 * HORIZONTAL_OFFSET);
        int horizontalOffset = (width() - numbOfButtons * buttonWidth ) / (numbOfButtons + 1);
        if(!title.isEmpty())
        {
            titleLabel->setGeometry(HORIZONTAL_OFFSET, VERTICAL_BORDER_OFFSET, referenceWidth, titleMetrics.boundingRect(0, 0, referenceWidth, MAX_TEST_RECT_HEIGHT, Qt::AlignHCenter | Qt::TextWordWrap, title).height() + 5);
            titleLabel->setAlignment(Qt::AlignCenter);
            titleLabel->setText(title);
            questionLabel->setAlignment(Qt::AlignCenter);
            questionLabel->setGeometry(HORIZONTAL_OFFSET, titleLabel->y() + titleLabel->height() + VERTICAL_OFFSET, referenceWidth, contentMetrics.boundingRect(0, 0, referenceWidth, MAX_TEST_RECT_HEIGHT, Qt::AlignHCenter | Qt::TextWordWrap, content).height() + 5);
            if(yesNoQuestion){
                yesButton->setAlignment(Qt::AlignCenter);
                yesButton->setGeometry(horizontalOffset , questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                yesButton->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                noButton->setAlignment(Qt::AlignCenter);
                noButton->setGeometry(width() - horizontalOffset - buttonWidth, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                noButton->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                setFixedHeight(yesButton->y() + BUTTONS_HEIGHT + VERTICAL_BORDER_OFFSET);
            }else{
                firstOptionLabel->setAlignment(Qt::AlignCenter);
                firstOptionLabel->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                secondOptionLabel->setAlignment(Qt::AlignCenter);
                secondOptionLabel->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                thirdOptionLabel->setAlignment(Qt::AlignCenter);
                thirdOptionLabel->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                switch(numbOfButtons){
                case 1:
                    delete secondOptionLabel;
                    secondOptionLabel = nullptr;
                    delete thirdOptionLabel;
                    thirdOptionLabel = nullptr;
                    firstOptionLabel->setGeometry(horizontalOffset, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    break;
                case 2:
                    delete thirdOptionLabel;
                    thirdOptionLabel = nullptr;
                    firstOptionLabel->setGeometry(horizontalOffset, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    secondOptionLabel->setGeometry(width() - horizontalOffset -  buttonWidth, questionLabel->y() + questionLabel->height() + VERTICAL_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    break;
                case 3:
                    firstOptionLabel->setGeometry(horizontalOffset, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    secondOptionLabel->setGeometry(2 * horizontalOffset + buttonWidth, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    thirdOptionLabel->setGeometry(width() - horizontalOffset - buttonWidth, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    break;
                }
                setFixedHeight(firstOptionLabel->y() + BUTTONS_HEIGHT + VERTICAL_BORDER_OFFSET);
            }
        }else{
            delete titleLabel;
            titleLabel = nullptr;
            questionLabel->setAlignment(Qt::AlignCenter);
            questionLabel->setGeometry(HORIZONTAL_OFFSET, VERTICAL_BORDER_OFFSET, referenceWidth, contentMetrics.boundingRect(0, 0, referenceWidth, MAX_TEST_RECT_HEIGHT, Qt::AlignHCenter | Qt::TextWordWrap, content).height() + 5);
            if(yesNoQuestion){
                yesButton->setAlignment(Qt::AlignCenter);
                yesButton->setGeometry(horizontalOffset, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                yesButton->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                noButton->setAlignment(Qt::AlignCenter);
                noButton->setGeometry(width() - horizontalOffset - buttonWidth, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                noButton->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                setFixedHeight(yesButton->y() + BUTTONS_HEIGHT + VERTICAL_BORDER_OFFSET);
            }else{
                firstOptionLabel->setAlignment(Qt::AlignCenter);
                firstOptionLabel->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                secondOptionLabel->setAlignment(Qt::AlignCenter);
                secondOptionLabel->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                thirdOptionLabel->setAlignment(Qt::AlignCenter);
                thirdOptionLabel->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
                switch(numbOfButtons){
                case 1:
                    delete secondOptionLabel;
                    secondOptionLabel = nullptr;
                    delete thirdOptionLabel;
                    thirdOptionLabel = nullptr;
                    firstOptionLabel->setGeometry(horizontalOffset, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    break;
                case 2:
                    delete thirdOptionLabel;
                    thirdOptionLabel = nullptr;
                    firstOptionLabel->setGeometry(horizontalOffset, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    secondOptionLabel->setGeometry(width() - horizontalOffset -  buttonWidth, questionLabel->y() + questionLabel->height() + VERTICAL_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    break;
                case 3:
                    firstOptionLabel->setGeometry(horizontalOffset, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    secondOptionLabel->setGeometry(2 * horizontalOffset + buttonWidth, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    thirdOptionLabel->setGeometry(width() - horizontalOffset -  buttonWidth, questionLabel->y() + questionLabel->height() + VERTICAL_BORDER_OFFSET, buttonWidth, BUTTONS_HEIGHT);
                    break;
                }
                setFixedHeight(firstOptionLabel->y() + BUTTONS_HEIGHT + VERTICAL_BORDER_OFFSET);
            }
        }
        if(App::compareStr(content, QString("Proszę czekać...")))
        {
            waitingTimer = new QTimer();
            waitingTimer->setInterval(1);
            connect(waitingTimer, SIGNAL(timeout()), this, SLOT(waitingTimerAction()));
            waitingTimer->start();
        }
        questionLabel->setText(content);
        if(!yesNoQuestion){
            firstOptionLabel->setText(setFirstOption);
            firstOptionLabel->installEventFilter(catchEvent);
            if(secondOptionLabel){
                secondOptionLabel->setText(setSecondOption);
                secondOptionLabel->installEventFilter(catchEvent);
            }
            if(thirdOptionLabel){
                thirdOptionLabel->setText(setThirdOption);
                thirdOptionLabel->installEventFilter(catchEvent);
            }
        }else{
            yesButton->setText("Tak");
            noButton->setText("Nie");
            yesButton->installEventFilter(catchEvent);
            noButton->installEventFilter(catchEvent);
        }
    }
        break;
    default:
        break;
    }
}

MyDialog::~MyDialog(){
    switch(type){
    case DEFAULT_PATH_DIALOG:
        break;
    case QUESTION_DIALOG:
        delete titleLabel;
        delete questionLabel;
        if(!yesNoQuestion){
            delete firstOptionLabel;
            delete secondOptionLabel;
            delete thirdOptionLabel;
        }else{
            delete yesButton;
            delete noButton;
        }
        break;
    default:
        delete infoLabel;
        delete acceptButton;
        delete cancelButton;
        break;
    }
    delete catchEvent;
    if(waitingTimer != nullptr)
        delete waitingTimer;
    delete scrollArea;
}

void MyDialog::createDialog(){
    QFont f("Comic Sans MS", 14, QFont::Medium);
    switch(type){
    case DEFAULT_PATH_DIALOG:
        setFixedSize(500, 115);
        infoLabel = new QLabel(this);
        infoLabel->setFont(f);
        infoLabel->setGeometry(190, 10, 120, 25);
        infoLabel->setAlignment(Qt::AlignCenter);
        infoLabel->setText(QString("Ustawienia"));
        infoLabel->show();
        projectPathInfo = new QLabel(QString("Ścieżka lokalna:"),this);
        projectPathInfo->setGeometry(10, 45, 80, 25);
        projectPathInfo->show();
        projectPathLabel = new QLabel(this);
        projectPathLabel->setText(static_cast<AppWindow*>(parent)->getApplication()->getConfiguration()->getDefaultPath());
        projectPathLabel->setGeometry(100, 45, projectPathLabel->fontMetrics().horizontalAdvance(projectPathLabel->text()), 25);
        scrollArea = new QScrollArea(this);
        scrollArea->setStyleSheet("background-color: transparent");
        scrollArea->setFrameShape(QFrame::NoFrame);
        scrollArea->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{background: transparent; height: 5px;} QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal, QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal, QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal{border: none; background: none; color: none;} QScrollBar::handle:horizontal{background-color: white; border: 1px solid black;}");
        scrollArea->setWidget(projectPathLabel);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->show();
        scrollArea->setGeometry(100, 45, 325, 25 + 5);
        fileDialogButton = new QLabel(this);
        fileDialogButton->setPixmap(QPixmap("Icons\\openProject.png"));
        fileDialogButton->setGeometry(430, 45, 25, 25);
        fileDialogButton->setAlignment(Qt::AlignCenter);
        fileDialogButton->show();
        resetToDefaultPathButton = new QLabel(this);
        resetToDefaultPathButton->setPixmap(QPixmap("Icons\\resetIcon.png"));
        resetToDefaultPathButton->setGeometry(465, 45, 25, 25);
        resetToDefaultPathButton->setAlignment(Qt::AlignCenter);
        resetToDefaultPathButton->show();
        acceptButton = new QLabel(QString("Zatwierdź"), this);
        acceptButton->setAlignment(Qt::AlignCenter);
        acceptButton->setGeometry(100, 80, 100, 25);
        acceptButton->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
        acceptButton->show();
        cancelButton = new QLabel(QString("Anuluj"), this);
        cancelButton->setAlignment(Qt::AlignCenter);
        cancelButton->setGeometry(300, 80, 100, 25);
        cancelButton->setStyleSheet("border: 1px solid grey; background-color: rgba(219, 217, 217, 0.9); border-radius: 5px;");
        cancelButton->show();
        acceptButton->installEventFilter(catchEvent);
        fileDialogButton->installEventFilter(catchEvent);
        resetToDefaultPathButton->installEventFilter(catchEvent);
        cancelButton->installEventFilter(catchEvent);
        break;
    case SUBSTANCE_FILE_ERRORS_LIST:
        // Bez Breaka (Postać DIALOGA podobna do Question Dialog)
    case QUESTION_DIALOG:
            titleLabel = new QLabel(this);
            titleLabel->show();
            questionLabel = new QLabel(this);
            questionLabel->show();
            if(yesNoQuestion){
                yesButton = new QLabel("Tak", this);
                yesButton->show();
                noButton =  new QLabel("Nie", this);
                noButton->show();
            }else{
                firstOptionLabel = new QLabel(this);
                firstOptionLabel->show();
                secondOptionLabel = new QLabel(this);
                secondOptionLabel->show();
                thirdOptionLabel = new QLabel(this);
                thirdOptionLabel->show();
            }
        break;
    }
}

void MyDialog::acceptButtonReleased(){
    switch(type){
    case DEFAULT_PATH_DIALOG:
        if(!App::compareStr(static_cast<AppWindow*>(parent)->getApplication()->getConfiguration()->getDefaultPath(), projectPathLabel->text() + "\\"))
        {
            if(!QDir(projectPathLabel->text()).exists()){
                MyDialog dlg(QUESTION_DIALOG, QString("Błąd lokalizacji"), QString("Podana ścieżka nie istnieje."), false, QString("Ok"), QString(), QString(), nullptr);
                dlg.setWindowModality(Qt::ApplicationModal);
                dlg.show();
                dlg.exec();
                return;
            }else{
                static_cast<AppWindow*>(parent)->getApplication()->getConfiguration()->setDefaultPath(projectPathLabel->text() + "\\");
                done(ACCEPT_RESULT);
            }
        }
        break;
    default:
        break;
    }
}

void MyDialog::fileDialogButtonReleased(){
    QString temp = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this,QString("Wybierz nową ścieżkę lokalną programu:"), QDir::homePath()));
    if(!temp.isEmpty()){
        projectPathLabel->setText(temp);
        projectPathLabel->resize(projectPathLabel->fontMetrics().horizontalAdvance(temp), projectPathLabel->height());
    }
}

void MyDialog::resetToDefaultPathButtonReleased(){
    projectPathLabel->setText(static_cast<AppWindow*>(parent)->getApplication()->getConfiguration()->getDefaultPath());
}

void MyDialog::cancelButtonReleased(){
    done(CANCEL_RESULT);
}

void MyDialog::yesButtonReleased(){
    done(YES_RESULT);
}

void MyDialog::noButtonReleased(){
    done(NO_RESULT);
}

void MyDialog::firstOptionLabelReleased(){
    done(FIRST_OPTION_RESULT);
}

void MyDialog::secondOptionLabelReleased(){
    done(SECOND_OPTION_RESULT);
}

void MyDialog::thirdOptionLabelReleased(){
    done(THIRD_OPTION_RESULT);
}

QLabel* MyDialog::getAcceptButton(){
    return acceptButton;
}

QLabel* MyDialog::getCancelButton(){
    return cancelButton;
}

QLabel* MyDialog::getYesButton(){
    return yesButton;
}

QLabel* MyDialog::getNoButton(){
    return noButton;
}

QLabel* MyDialog::getFirstOptionLabel(){
    return firstOptionLabel;
}

QLabel* MyDialog::getSecondOptionLabel(){
    return secondOptionLabel;
}

QLabel* MyDialog::getThirdOptionLabel(){
    return thirdOptionLabel;
}

bool MyDialog::getYesNoQuestion(){
    return yesNoQuestion;
}

DialogType MyDialog::getType(){
    return type;
}

QLabel* MyDialog::getFileDialogButton(){
    return fileDialogButton;
}

QLabel* MyDialog::getResetToDefaultPathButton(){
    return resetToDefaultPathButton;
}

void MyDialog::setExitCode(DialogResult* dlgExitCode){
    if(exitCode != nullptr)
        delete exitCode;
    exitCode = dlgExitCode;
}

void MyDialog::waitingTimerAction(){
    if(exitCode != nullptr){
        done(*exitCode);
    }
}

bool MyDialog::checkFileSFEL(QFile* file){
    if(!file->open(QFile::Text | QFile::ReadOnly))
    {
        MyDialog dlg(QUESTION_DIALOG, QString("Błąd otwarcia pliku"), QString("Wystąpił błąd przy próbie otwarcia pliku."), false, QString("Ok"), QString(), QString(), nullptr);
        dlg.setWindowModality(Qt::ApplicationModal);
        dlg.show();
        dlg.exec();
        delete file;
        QDialog::deleteLater();
        return false;
    }
    char tempC = 0;
    QString tempStr;
    bool presDep = false;
    bool firstPressure = false;
    bool firstTemp = false;
    bool firstData = false;
    bool nextPresOrTemp = false;
    bool nextData = false;
    double lastPressure = 0;
    QString lastPressureS;
    double lastTemp = 0;
    QString lastTempS;
    double lastData = 0;
    bool decresing = 0;
    bool difrenceMeas = 0;
    while(!file->atEnd()){
        tempStr.clear();
        file->getChar(&tempC);
        switch(tempC){
        case 'z':
            file->getChar(&tempC);
            file->getChar(&tempC);
            file->getChar(&tempC);
            file->getChar(&tempC);
            presDep = (tempC == '1')? 0 : 1;
            break;
        case 'k':
            file->getChar(&tempC);
            file->getChar(&tempC);
             while(tempC != '\n'){
                file->getChar(&tempC);
            }
            nextPresOrTemp = 1;
            break;
        case 'p':
            if(presDep)
            {
            file->getChar(&tempC);
            if(tempC != '='){
                while(tempC != '=')
                    file->getChar(&tempC);
                break;
            }
            file->getChar(&tempC);
            while(tempC != '\n' && tempC != ' '){
                if(tempC < 48 || tempC > 57 ){
                    if(tempC == '-'){
                        tempStr.append(tempC);
                    }else{
                        if(tempC == '.'){
                            tempStr.append(tempC);
                        }
                    }
                }else{
                    tempStr.append(tempC);
                }
                file->getChar(&tempC);
            }
            if(firstPressure && lastPressure >= tempStr.toDouble()){
                ;
            }else{
                firstPressure = true;
                decresing = 0;
                lastPressure = tempStr.toDouble();
                lastPressureS = tempStr;
            }
            firstData = 0;
            firstTemp = 0;
            nextPresOrTemp = 1;
            nextData = 0;
        }
            break;
        case 'T':
            file->getChar(&tempC);
            if(tempC != '='){
                while(tempC != '=')
                    file->getChar(&tempC);
                break;
            }
            file->getChar(&tempC);
            while(tempC != '\n' && tempC != ' '){
                if(tempC < 48 || tempC > 57 ){
                    if(tempC == '-'){
                        tempStr.append(tempC);
                    }else{
                        if(tempC == '.'){
                            tempStr.append(tempC);
                        }
                    }
                }else{
                    tempStr.append(tempC);
                }
                file->getChar(&tempC);
            }
            firstTemp = true;
            lastTemp = tempStr.toDouble();
            lastTempS = tempStr;
            nextData = 1;
            nextPresOrTemp = 0;
            break;
        case 'r':
            file->getChar(&tempC);
            if(tempC != '='){
                while(tempC != '=')
                    file->getChar(&tempC);
                break;
            }
            file->getChar(&tempC);
            while(tempC != '\n' && tempC != ' '){
                if(tempC < 48 || tempC > 57 ){
                    if(tempC == '-'){
                        tempStr.append(tempC);
                    }else{
                        if(tempC == '.'){
                            tempStr.append(tempC);
                        }
                    }
                }else{
                    tempStr.append(tempC);
                }
                file->getChar(&tempC);
            }
            if(difrenceMeas){
                if(lastData != tempStr.toDouble()){
                    if(lastData > tempStr.toDouble())
                        decresing = 1;
                    difrenceMeas = 0;
                }
            }
            if(firstData && !difrenceMeas){
                if(decresing){
                     if((lastData < tempStr.toDouble())){
                         if(presDep)
                             substanceFileErrorsList->addElement(lastPressureS, lastTempS);
                         else
                             substanceFileErrorsList->addElement(lastTempS);
                     }
                 }else{
                    if((lastData > tempStr.toDouble())){
                        if(presDep)
                            substanceFileErrorsList->addElement(lastPressureS, lastTempS);
                        else
                            substanceFileErrorsList->addElement(lastTempS);

                    }
                }
                lastData = tempStr.toDouble();
            }else{
                if(!firstData)
                    difrenceMeas = 1;
                firstData = true;
                lastData = tempStr.toDouble();
            }
            nextData = 0;
            nextPresOrTemp = 1;
            break;
        case 'n':
            file->getChar(&tempC);
            if(tempC != '='){
                while(tempC != '=')
                    file->getChar(&tempC);
                break;
            }
            file->getChar(&tempC);
            while(tempC != '\n' && tempC != ' '){
                if(tempC < 48 || tempC > 57 ){
                    if(tempC == '-'){
                        tempStr.append(tempC);
                        }else{
                        if(tempC != '.'){
                            tempStr.append(tempC);
                        }
                    }
                }else{
                    tempStr.append(tempC);
                }
                file->getChar(&tempC);

            }
            if(difrenceMeas){
                if(lastData != tempStr.toDouble()){
                    if(lastData > tempStr.toDouble())
                        decresing = 1;
                    difrenceMeas = 0;
                }
            }
            if(firstData && !difrenceMeas){
                if(decresing){
                     if((lastData < tempStr.toDouble())){
                         if(presDep)
                             substanceFileErrorsList->addElement(lastPressureS, lastTempS);
                         else
                             substanceFileErrorsList->addElement(lastTempS);
                     }
                 }else{
                    if((lastData > tempStr.toDouble())){
                        if(presDep)
                            substanceFileErrorsList->addElement(lastPressureS, lastTempS);
                        else
                            substanceFileErrorsList->addElement(lastTempS);
                    }
                }
                lastData = tempStr.toDouble();
            }else{
                if(!firstData)
                    difrenceMeas = 1;
                firstData = true;
                lastData = tempStr.toDouble();
            }
            nextData = 0;
            nextPresOrTemp =1;
            break;
        case ' ':
            break;
        case '\r':
            break;
        case '\n':
            break;
        default:
            break;
        }
    }
    file->close();
    delete file;
    if(substanceFileErrorsList->getNumbOfElem() == 0){
        QDialog::deleteLater();
        return false;
    }else{
        scrollArea->setWidget(substanceFileErrorsList);
        show();
    }
    return  true;
}

QScrollArea* MyDialog::getScrollArea(){
    return scrollArea;
}

SubstanceFileErrorsList* MyDialog::getSubstanceFileErrorsList(){
    return substanceFileErrorsList;
}

MaterialsChangesWidget<SubstancesParamsFileParam>* MyDialog::getMCW(){
    return mcw;
}
