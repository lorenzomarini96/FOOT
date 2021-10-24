// Funzione che printa la lista dei file di una data 
// estensione e situati in una data cartella

void loop_file_dir1(const char *dirname, const char *ext)
{
    TSystemDirectory dir(dirname, dirname);
    TList *files = dir.GetListOfFiles();
    if (files)
    {
        TSystemFile *file;
        TString fname;
        TIter next(files);
        while ((file=(TSystemFile*)next()))
        {
            fname = file->GetName();
            if (!file->IsDirectory() && fname.EndsWith(ext))
            {
                cout << fname.Data() << endl;
            }
        }

    }

}

