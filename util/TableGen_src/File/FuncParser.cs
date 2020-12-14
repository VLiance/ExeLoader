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
		 List<string> aCppLineOpt = new List<string>();



        public FuncParser(FileText _oFile){
            oFile = _oFile;
        }

        public void parse() {
            parse_normalize();
            parse_optimize();

            FileWritter _oFileResult = new FileWritter("Out.txt");
		//	_oFileResult.writeFile(aCppLine);
			_oFileResult.writeFile(aCppLineOpt);

            Log.debug("!!FINISH!!");
        }

       

        //First pass remove comment & remove line break, split with ;
        public void parse_normalize() {
			bool _bInsideMultilineComment = false;
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
				if(idx < _sLine.Length && _sLine[idx] == '#') {idx++;
					if(_sExtLine != "") {aCppLine.Add(_sExtLine);_sExtLine=""; //saveline
					}_sExtLine+='#';
                    idx=Str.skipspace(_sLine, idx);
					_bSingleLineMode = true;
				}

				//Find comments
				for(; idx < _sLine.Length; idx++) {
                    char currChar = _sLine[idx];
                  
					if(!_bInsideMultilineComment) {
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
							_sExtLine = _sExtLine.Substring(0, _sExtLine.Length-1);//revert
							_bInsideMultilineComment = true;
						}
						else if( currChar == ';' && !_bSingleLineMode) {
							_sExtLine += currChar;//Keep ';' ?
							aCppLine.Add(_sExtLine);_sExtLine="";lastChar=' '; //saveline
						}else { 
							_sExtLine += currChar;
						}
					}else {
						if(lastChar == '*' && currChar == '/') {
							_bInsideMultilineComment = false;
							lastChar = ' ';
						}
					}
					lastChar=currChar;
				}
				if(_bSingleLineMode && _sExtLine != "") {
					if(_sExtLine[_sExtLine.Length-1] == '\\') { //Special char to make multiline '\'
						_sExtLine = _sExtLine.Substring(0, _sExtLine.Length-1); //revert
					}else {
						_bSingleLineMode = false;
						aCppLine.Add(_sExtLine);_sExtLine=""; //saveline
					}
				}
            }
        }


         //Second pass remove #if 0 / #endif
        public void parse_optimize() {
            int _scope_NotUsed = 0;
            int _scope_preproc = 1;//1 = global scope
            foreach(string __sLine in aCppLine) {Str _sLine = new Str(__sLine);
                if(_sLine.Cmp("#if")) {
                    _scope_preproc++;
                }
                else if(_sLine.Cmp("#endif")) {
                    if(_scope_NotUsed == _scope_preproc) {_scope_NotUsed=0;}//back to normal
                    _scope_preproc--;
                }

                if(_sLine.Cmp("#if 0")) {
                   _scope_NotUsed = _scope_preproc;
                }
                if(_scope_NotUsed == 0) {
                    aCppLineOpt.Add(_sLine.str);
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
