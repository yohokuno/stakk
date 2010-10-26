struct Server { 
    int port;

    // implement in sub class
    virtual wstring get_result(vector<string> path) = 0;

    // set locale
    Server() {
        setlocale(LC_CTYPE, "");
        locale::global(locale(""));
        port = 54633;
    }

    //parse http request
    vector<string> parse_request(string request) {
        vector<string> splited = split(request, '\n');
        vector<string> path;
        for (int i = 0; i < splited.size(); i++) {
            if (splited[i].find("GET") == 0) {
                vector<string> parsed = split(splited[i], ' ');
                if (parsed.size() < 1)
                    continue;
                path = split(parsed[1], '/');
                for (int j = 0; j < path.size(); j++) {
                    path[j] = urldecode(path[j]);
                }
            }
        }
        return path;
    }

    //generate http response
    wstring generate_response(wstring result) {
        wstring header = L"HTTP/1.1 200 OK\r\n";
        header += L"Content-Type:text/plain; charset=UTF-8\r\n";
        return header + L"\r\n" + result;
    }

    //socket communication
    int communicate() {
        int sock0;
        struct sockaddr_in addr;
        int yes = 1;

        sock0 = socket(AF_INET, SOCK_STREAM, 0);
        if (sock0 < 0) {
            perror("socket");
            return 1;
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        setsockopt(sock0, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

        if (bind(sock0, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
            perror("bind");
            return 1;
        }

        if (listen(sock0, 5) != 0) {
            perror("listen");
            return 1;
        }

        //main loop
        while (true) {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int sock = accept(sock0, (struct sockaddr *)&client, &len);
            if (sock < 0) {
                perror("accept");
                break;
            }

            char inbuf[2048];
            memset(inbuf, 0, sizeof(inbuf));
            recv(sock, inbuf, sizeof(inbuf), 0);
            vector<string> path = parse_request(inbuf);

            //process query and get result
            wstring result = get_result(path);
            wstring response = generate_response(result);
            //wcout << L"output: " << response << endl;

            string output = narrow(response);
            send(sock, output.c_str(), (int)strlen(output.c_str()), 0);

            close(sock);
        }
        close(sock0);
        return 0;
    }
};
