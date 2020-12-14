using App;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace App
{
    public class FuncParser
    {
        FileText oFile;
        List<string> aProtoList = new List<string>();

		 List<string> aCppLine = new List<string>();
		 List<string> aCppLine_Opt = new List<string>();

		 List<string> aCppLine_GlobalScope = new List<string>();


        public FuncParser(FileText _oFile){
            oFile = _oFile;
        }

        public void parse() {
            parse_normalize();
            parse_optimize();
            parse_gblscope();

            FileWritter _oFileResult = new FileWritter("Out.txt");
		//	_oFileResult.writeFile(aCppLine);
			_oFileResult.writeFile(aCppLine_Opt);
			//_oFileResult.writeFile(aCppLine_GlobalScope);
            
            Log.debug("!!FINISH!!");
        }

       
        public static void saveLine(List<string> _aTo, string _sLine) {
            string _sCleaned = _sLine.Trim();
            if(_sCleaned != "") {
                 _aTo.Add(_sLine.Trim());
            }
        }


        //First pass remove comment & remove line break, split with ;
        public void parse_normalize() {
			bool _bInsideMultilineComment = false;
			bool _bInsideStringLiteral = false;
			bool _bSingleLineMode = false;
			string _sExtLine = "";

            oFile.readFile();

			//First pass remove comment & remove line break, split with ;
            foreach(string _sLine in oFile.aLine){
				
				char lastChar = ' ';

				//Trim start
				int idx = 0;
                idx=Str.skipspace(_sLine, idx);
			    
				//Directive are on sigle line
				if(idx < _sLine.Length && _sLine[idx] == '#' && !_bInsideMultilineComment) {idx++;
					if(_sExtLine != "" && _sExtLine != " ") {saveLine(aCppLine, _sExtLine);_sExtLine=""; //saveline
					}_sExtLine+='#';
                    idx=Str.skipspace(_sLine, idx);
					_bSingleLineMode = true;
				}

				//Find comments
                char currChar = ' ';
				for(; idx < _sLine.Length; idx++) {
                    lastChar=currChar;
                    currChar = _sLine[idx];

					if(!_bInsideMultilineComment && !_bInsideStringLiteral) {
                        if(currChar <= 32) {  //32 = ascii table space ' '
                            //Normalise space
                            idx=Str.skipspace(_sLine, idx)-1;
                            currChar =  ' ';
                           	_sExtLine += currChar;
                        }
						else if(lastChar == '/' && currChar == '/') {
							_sExtLine = _sExtLine.Substring(0, _sExtLine.Length-1); //revert
							break;
						}
						else if(lastChar == '/' && currChar == '*') {
							_sExtLine = _sExtLine.Substring(0, _sExtLine.Length-1); //revert
							_bInsideMultilineComment = true;
						}
						else if((currChar == ';' || currChar == '{' || currChar == '}') && !_bSingleLineMode) {
                            if(currChar != ';') {//; on same line
                                saveLine(aCppLine, _sExtLine);_sExtLine="";lastChar=' '; //saveline
                            }
							_sExtLine += currChar;//Keep ';' ?
                             saveLine(aCppLine, _sExtLine);_sExtLine="";lastChar=' '; //saveline
                        }
                        else if(currChar == '"' && lastChar != '\\') {
                              	_bInsideStringLiteral = true;
                                _sExtLine += currChar;
						}else { 
							_sExtLine += currChar;
						}
					}else if(_bInsideMultilineComment) {
						if(lastChar == '*' && currChar == '/') {
							_bInsideMultilineComment = false;
							lastChar = ' ';
						}
					}else if(_bInsideStringLiteral) {
                        _sExtLine += currChar;
                        if(currChar == '"' && lastChar != '\\' ) {
							_bInsideStringLiteral = false;
							lastChar = ' ';
						}
                    }
				}

				if(_bSingleLineMode) {
					if(_sExtLine != "" && _sExtLine[_sExtLine.Length-1] == '\\') { //Special char to make multiline '\'
						_sExtLine = _sExtLine.Substring(0, _sExtLine.Length-1); //revert
					}else {
						_bSingleLineMode = false;
						saveLine(aCppLine, _sExtLine);_sExtLine="";lastChar=' '; //saveline
					}
				}else { 
                    if(!_bSingleLineMode && lastChar > 32) { //32 = ascii table space ' '
                        _sExtLine += ' ';
                    }
                }
            }
            aCppLine.Add(" ");//Normalise with a empty ending line (to easily get next line)
        }


         //Second pass remove #if 0 / #endif
        public void parse_optimize() {
            int _scope_NotUsed = 0;
            int _scope_preproc = 1;//1 = global scope
            foreach(string __sLine in aCppLine) {Str _sLine = new Str(__sLine);
                bool _bKeepLine = false;

                if(_sLine.Cmp("#if")) {
                    _scope_preproc++;
                }

                if(_sLine.Cmp("#if 0")) {
                  if(_scope_NotUsed == 0) { _scope_NotUsed = _scope_preproc; _bKeepLine = true;}
                }

                if(_sLine.Cmp("#else")) {
                    if(_scope_NotUsed == _scope_preproc) {_scope_NotUsed=0;_bKeepLine = true;}//back to normal
                }

               if(_sLine.Cmp("#endif")) {
                    if(_scope_NotUsed == _scope_preproc) {_scope_NotUsed=0;_bKeepLine = true;}//back to normal
                    _scope_preproc--;
                }

                if(_bKeepLine || _scope_NotUsed == 0) {
                    aCppLine_Opt.Add(_sLine.str);
                }else {
                    //aCppLine_Opt.Add("//" + _sLine.str);
                }

            }
        }

  
        public void parse_gblscope() {
             int _scope = 1;//1 = global scope
            foreach(string __sLine in aCppLine_Opt) {Str _sLine = new Str(__sLine);
                if(_sLine.str[0] == '{') {
                    _scope++;
                }


                if(_scope == 1) {
                    aCppLine_GlobalScope.Add(_sLine.str);
                }else {
                     aCppLine_GlobalScope.Add("//" + _sLine.str);
                }



                if(_sLine.str[0] == '}') {
                    _scope--;
                    if(_scope == 1) {  aCppLine_GlobalScope.Add(";"); }
                }


            }


        }
        

		public bool is_funcProto(string _sLine) { //Line must be trimmed & not empty (min 4 char)
			//not directive
			if (_sLine[0] == '#') {return false;}
			//must end with ;
			if(_sLine[_sLine.Length-1] != ';') {return false;}



			return false;
		}

		public string extract_FuncProto(string _sLine) {


			return _sLine;
		}

    }
}
