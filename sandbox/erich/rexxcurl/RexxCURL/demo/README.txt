The Rexx/CURL demo programs are:
- certinfo.rexx Retrieve Certificates
  Duplication of cURL example; certinfo.c
  Retrieves Certificate information from a web site.

- getright.rexx URL output_directory
  Simplistic implementation of the "GetRight" download utility. Downloads
  the specified URL to the specified directory. If the file already exists,
  it will use the RESUME capability of the server (if implemented)

- httppost.rexx your_email_address your_list_password
  Suscribes to the rexxcurl mailing list at SourceForge using HTTP POST to fill
  in an HTML form.

- sendmail-smtp.rexx
  Sends text email via SMTP to a destination using a GMail account. Changes required
  to the code to supply the correct GMail credentials.

- getmail-pop3.rexx
  Reads email from a POP3 mail server. Changes required
  to the code to specify the correct email account details.

- scp.rexx remote_file_spec local_file_spec
- scp.rexx local_file_spec remote_file_spec
  Mimics the SSH program: scp, but only copies to/from an ftp server

- testcurl.rexx URL
  Displays the HTML source of the specified URL

- upload.rexx filenames
  Uploads the files specified to the SourceForge "uploads" directory
