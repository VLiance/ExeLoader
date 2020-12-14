#region header
// ========================================================================
// Copyright (c) 2018 - Julien Caillon (julien.caillon@gmail.com)
// This file (SubString.cs) is part of YamuiFramework.
// 
// YamuiFramework is a free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// YamuiFramework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with YamuiFramework. If not, see <http://www.gnu.org/licenses/>.
// ========================================================================
#endregion
using System;

namespace App
{
    public class Str {

        public readonly string str;
        public readonly int size;

        public Str(string _str) {
            str = _str;
            size = _str.Length;
        }


        public static int skipspace(string _sLine, int idx) {
             for(; idx < _sLine.Length; idx++) {if(_sLine[idx] > 32) {break;}}  //32 = ascii table space ' '
             return idx;
        }

        public string SubStr(int _startIdx, int _endIdx) {
           return "";
        }

        public bool Cmp(string _cmp, int _startIdx = 0) {
            if(size-_startIdx < _cmp.Length) {return false;}
            for(int i=0; i< _cmp.Length; i++) {
                if( str[_startIdx+i] !=_cmp[i]) {return false;}
            }
           return true;
        }

        
        public char this[int idx] {
            get {
                if (idx < 0 || idx > size){
                    return (char)0;
                }
                return str[idx];
            }
        }

        public bool IsEmpty() {
            return size < 1;
        }

        public bool IsEmptyOrWhitespace() {
            for (int i = 0; i < size; i++) {
                if (!char.IsWhiteSpace(str,  i))
                    return false;
            }
            return true;
        }

  
    }
}