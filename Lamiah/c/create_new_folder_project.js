var arguments = process.argv

var node_binary = arguments.shift()
var script_path_filename = arguments.shift()

if(arguments.length == 0){
    console.warn('no foldername specified, do it with:')
    console.info(`${node_binary.split('/').pop()} ${script_path_filename.split('/').pop()} the_folder_name_here`)
    process.exit(1)
}

var folder_name = arguments.shift()

var fs = require('fs');
var dir = './'+folder_name;

if (!fs.existsSync(dir)){
    
    fs.mkdirSync(dir);
   
}else{
    //console.log(`directory : ${folder_name} already exists`)
}


 
var makefile_content = `
CFLAGS = -Wall -g -O3

${folder_name}: ${folder_name}.c
\tgcc $(CFLAGS) ${folder_name}.c -o ${folder_name}

clean:
\trm -f ${folder_name}

`;
var c_file_content = `
// c file 
#include <stdio.h>

int main() {
   /* my first program in C */
   printf("main function called");
   
   return 0;
}
`
var c_file_filename = dir+".c"
var makefile_filename = "Makefile"
if (!fs.existsSync(dir+'/'+makefile_filename)){

    fs.writeFile(dir+'/'+makefile_filename, makefile_content, function(err) {
        if(err) {
            return console.log(err);
        }
        console.log("The "+dir+'/'+makefile_filename+" file was saved!");
    }); 

}
if (!fs.existsSync(dir+'/'+c_file_filename)){

    fs.writeFile(dir+'/'+c_file_filename, c_file_content, function(err) {
        if(err) {
            return console.log(err);
        }
        console.log("The "+dir+'/'+c_file_filename+" file was saved!");
    }); 

}

console.log('done')