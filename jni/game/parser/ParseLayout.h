#pragma once

class ParseLayout
{
public:
  char *pfileName;

public:
  ParseLayout();
  ~ParseLayout();
  void setFileName(char *pfileName_)
  {
    pfileName = pfileName_;
  }

  void parse()
  {
  }
};
