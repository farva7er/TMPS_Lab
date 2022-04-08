#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Input.H>

#include <string>
#include <iostream>
#include <curl/curl.h>
#include <stdio.h>

const char* username = "mailtest";
const char* password = "mailtestpass";

CURL *curl;

enum
{
   LeftBarWidth = 200,
   TextDisplayWidth = 600,
   WinHeight = 800

};

unsigned int count_lines(const char* buff) {
   if(strlen(buff) < 3)
      return 0;

   int newlines = 0;
   for (; *buff; buff++) {
      if ( *buff == '\n'  ) {
         newlines++;
      }
   }
   return newlines;

}

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
       ((std::string*)userp)->append((char*)contents, size * nmemb);
           return size * nmemb;
           
}

struct upload_status {
   size_t bytes_read;

};

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
   std::string& str = *(std::string*)userp;

   size_t room = size * nmemb;

   if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
      return 0;


   }

   if(str.length() > 0) {
      size_t len = str.length();
      if(room < len)
         len = room;
      memcpy(ptr, str.c_str(), len);
      str.erase(0, len);
      return len;
   }

   return 0;
}


class Mail
{
public:
   Mail(const std::string& from, const std::string& to,
         const std::string& body)
      : m_From(from), m_To(to), m_Body(body)
   {}

   const std::string& GetFrom() const
   {
      return m_From;
   }

   const std::string& GetTo() const
   {
      return m_To;
   }

   const std::string& GetBody() const
   {
      return m_Body;
   }
   
private:
   std::string m_From;
   std::string m_To;
   std::string m_Body;
};



class Mailer
{
public:
   static int GetMailCount()
   {
      std::string readBuffer = "";
      CURL *curl;
      CURLcode res = CURLE_OK;

      curl = curl_easy_init();
      if(curl) {
         /* Set username and password */
         curl_easy_setopt(curl, CURLOPT_USERNAME, username);
         curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

         /* This will retrieve message 1 from the user's mailbox */
         curl_easy_setopt(curl, CURLOPT_URL, "pop3://mail.farvater.xyz/");

         curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

         /* Perform the retr */
         res = curl_easy_perform(curl);

         /* Check for errors */
         if(res != CURLE_OK)
            fprintf(stderr, " 1 curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

         if(CURLE_OK == res) {
            int count = count_lines(readBuffer.c_str());
            //printf("Counted: %d\n", count);
            return count;
         }

         /* Always cleanup */
         curl_easy_cleanup(curl);
         return 0;
      }
      return 0; 
   }

   static Mail* GetMail(int i)
   {
      std::string readBuffer;
      char num[10];
      sprintf(num, "%d", i);
      std::string url = "pop3://mail.farvater.xyz/";
      url += num;

      //printf("Get Mail %d\n", i);
      CURLcode res = CURLE_OK;
      if(curl) {
         /* Set username and password */
         curl_easy_setopt(curl, CURLOPT_USERNAME, username);
         curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

         /* This will retrieve message 1 from the user's mailbox */
         curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

         curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

         /* Perform the retr */
         res = curl_easy_perform(curl);

         /* Check for errors */
         if(res != CURLE_OK)
            fprintf(stderr, "2 curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

         if(CURLE_OK == res) {

            return new Mail("", "", readBuffer);
         }

         return nullptr;
      }
      return nullptr; 
   }

   static int SendMail(Mail* mail)
   {
      std::string* payload_text = new std::string(
         "To: " + mail->GetTo() + "\r\n"
         "From: " + mail->GetFrom() + "\r\n"
         "Subject: SMTP example message\r\n"
         "\r\n" /* empty line to divide headers from body, see RFC5322 */
         + mail->GetBody());


      CURLcode res = CURLE_OK;
      struct curl_slist *recipients = NULL;

      if(curl) {
         /* Set username and password */
         curl_easy_setopt(curl, CURLOPT_USERNAME, username);
         curl_easy_setopt(curl, CURLOPT_PASSWORD, password);

         curl_easy_setopt(curl, CURLOPT_URL, "smtp://mail.farvater.xyz:587");

         curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

         curl_easy_setopt(curl, CURLOPT_MAIL_FROM, mail->GetFrom().c_str());

         recipients = curl_slist_append(recipients, mail->GetTo().c_str());

         curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

         /* We are using a callback function to specify the payload (the headers and
          *        *      * body of the message). You could just use the CURLOPT_READDATA option to
          *               *           * specify a FILE pointer to read from. */
         curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
         curl_easy_setopt(curl, CURLOPT_READDATA, payload_text);
         curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

         /* Since the traffic will be encrypted, it is very useful to turn on debug
          *        *      * information within libcurl to see what is happening during the transfer.
          *               *           */
         //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

         /* Send the message */
         res = curl_easy_perform(curl);

         /* Check for errors */
         if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                  curl_easy_strerror(res));

         /* Free the list of recipients */
         curl_slist_free_all(recipients);

         if(res == CURLE_OK)
         {
            return 0;
         }
         return 1; 
      }
      return 1;
   }
};


class EmailClientWindow : public Fl_Window
{
public:
   EmailClientWindow(int leftBarWidth, int textDisplayWidth, int winHeight)
      : Fl_Window(leftBarWidth + textDisplayWidth, winHeight, "Email Client"),
         m_LeftBarWidth(leftBarWidth), m_TextDisplayWidth(textDisplayWidth),
         m_WinHeight(winHeight), m_IncomingMail(nullptr)
   {

      m_TextDisplayBuff = new Fl_Text_Buffer();
      m_TextDisplay = new Fl_Text_Display(m_LeftBarWidth, 0,
                                          m_TextDisplayWidth, m_WinHeight);
      m_TextDisplay->buffer(m_TextDisplayBuff);

      m_TextEditorBuff = new Fl_Text_Buffer();
      m_TextEditor = new Fl_Text_Editor(m_LeftBarWidth, 60,
                                          m_TextDisplayWidth, m_WinHeight);
      m_TextEditor->buffer(m_TextEditorBuff);
      m_TextEditorBuff->text("Hello world!");
      m_TextEditor->hide();

      m_FromInput = new Fl_Input(m_LeftBarWidth + 50, 10, m_TextDisplayWidth - 150, 20);
      m_FromInput->label("From: ");
      m_ToInput = new Fl_Input(m_LeftBarWidth + 50, 30, m_TextDisplayWidth - 150, 20);
      m_ToInput->label("To: ");

      m_FromInput->hide();
      m_ToInput->hide();

      m_NewMailButton = new Fl_Button(10, 10, m_LeftBarWidth - 20, 50, "Write new Email");
      m_NewMailButton->callback(NewMailButtonCallback, this);
      m_NewMailButton->box(FL_PLASTIC_UP_BOX);

      m_SendMailButton = new Fl_Button(10, 10, m_LeftBarWidth - 20, 50, "Send Email");
      m_SendMailButton->callback(SendMailButtonCallback, this);
      m_SendMailButton->box(FL_PLASTIC_UP_BOX);
      m_SendMailButton->hide();

      m_FetchMailButton = new Fl_Button(10, 70, m_LeftBarWidth - 20, 50, "Fetch Email");
      m_FetchMailButton->callback(FetchMailButtonCallback, this);
      m_FetchMailButton->box(FL_PLASTIC_UP_BOX);

      m_LeftBarScroll = new Fl_Scroll(10, 140, m_LeftBarWidth - 20, m_WinHeight - 140);
      m_LeftBarScroll->end();

   }

   struct ReadMailCbData
   {
      EmailClientWindow* w;
      Mail* mail;
      ReadMailCbData(EmailClientWindow* w, Mail* mail)
         :w(w), mail(mail) {}
   };

   void AddIncomingMail(Mail* mail)
   {
      if(!mail)
         return;

      int i = 2;
      MailNode* curr = m_IncomingMail;
      while(curr)
      {
         m_LeftBarScroll->scroll_to(0, 0);
         char buff[10];
         sprintf(buff, "%d", i);
         curr->m_MailButton->copy_label(buff);
         curr->m_MailButton->position(10, 140 + (i-1)*80);
         i++;
         curr = curr->m_Next;
      }
      m_LeftBarScroll->begin();
      Fl_Button* b = new Fl_Button(10, 140, m_LeftBarWidth - 60, 50, "1");
      ReadMailCbData* cbData = new ReadMailCbData(this, mail);
      b->callback(ReadMailCallback, cbData);
      m_LeftBarScroll->end();
      m_LeftBarScroll->redraw();

      m_IncomingMail = new MailNode(mail, b, m_IncomingMail);
   }

   static void ReadMailCallback(Fl_Widget* w, void* userdata)
   {
      ReadMailCbData* m = (ReadMailCbData*)userdata;
      m->w->m_TextEditor->hide();
      m->w->m_FromInput->hide();
      m->w->m_ToInput->hide();
      m->w->m_SendMailButton->hide();
      m->w->m_NewMailButton->show();
      m->w->m_TextDisplayBuff->text(m->mail->GetBody().c_str());
      m->w->m_TextDisplay->show();
   }

   static void NewMailButtonCallback(Fl_Widget* w, void* userdata)
   {
      EmailClientWindow* client = (EmailClientWindow*)userdata;
      client->WriteNewMail();
   }

   static void FetchMailButtonCallback(Fl_Widget* w, void* userdata)
   {

      EmailClientWindow* client = (EmailClientWindow*)userdata;

      client->ClearIncomingMail();
      int count = Mailer::GetMailCount();
      for(int i = 1; i <= count; i++)
      {
         client->AddIncomingMail(Mailer::GetMail(i));
      }
   }

   static void SendMailButtonCallback(Fl_Widget* w, void* userdata)
   { 
      EmailClientWindow* client = (EmailClientWindow*)userdata;
      char * body = client->m_TextEditorBuff->text();
      Mail* mail = new Mail(client->m_FromInput->value(),
                           client->m_ToInput->value(),
                           body);
      Mailer::SendMail(mail);

      free(body);
      delete mail;
   }

   void ClearIncomingMail()
   {
      delete m_IncomingMail;
      m_IncomingMail = nullptr;
   }

   void WriteNewMail()
   {
      m_TextDisplay->hide();
      m_NewMailButton->hide();
      m_SendMailButton->show();
      m_TextEditor->show();
      m_FromInput->show();
      m_ToInput->show();
   }

private:

   int m_LeftBarWidth;
   int m_TextDisplayWidth;
   int m_WinHeight;

   Fl_Scroll* m_LeftBarScroll;

   Fl_Text_Buffer* m_TextDisplayBuff;
   Fl_Text_Display* m_TextDisplay;

   Fl_Text_Buffer* m_TextEditorBuff;
   Fl_Text_Editor* m_TextEditor;

   Fl_Input* m_FromInput;
   Fl_Input* m_ToInput;

   Fl_Button* m_NewMailButton;
   Fl_Button* m_SendMailButton;
   Fl_Button* m_FetchMailButton;

   struct MailNode
   {
      Mail* m_MailData;
      Fl_Button* m_MailButton;
      MailNode* m_Next;

      MailNode(Mail* mail, Fl_Button* b, MailNode* next = nullptr)
         : m_MailData(mail), m_MailButton(b), m_Next(next)
      {}
      ~MailNode()
      {
         delete m_MailData;
         Fl::delete_widget(m_MailButton);
         delete m_Next;
      }
   };

   MailNode* m_IncomingMail;
};

int main()
{
   Fl_Window *win = 
      new EmailClientWindow(LeftBarWidth, TextDisplayWidth, WinHeight);

   win->end();
   win->show();

   curl = curl_easy_init();
   Fl::run();
   /* Always cleanup */
   curl_easy_cleanup(curl);

   return Fl::run();
}
