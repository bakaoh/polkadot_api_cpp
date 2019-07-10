class IApplication {
public:
    virtual ~IApplication() {}

    /**
     *  Connects to WebSocket
     *
     * In order to establish successful TLS connection, a root CA certificate needs to be present in pem file configured
     * in CConstants::certificate_file (currently ca-chain.cert.pem). You can put several certificates in this text file
     * one after the other. Currently Polkadot poc-3 server is using DST_Root_CA_X3 CA, so the content of this
     * certificate file is added to ca-chain.cert.pem, but in case if this changes, one can find out the issuer
     * certificate by executing following these instructions:
     *
     *  1. Execute `curl --insecure -v https://poc3-rpc.polkadot.io:443`
     *  2. Find "issuer" line in the output and download issuer certificate from their website
     *  3. Execute `openssl x509 -in <downloaded_cert_file> -noout -issuer`
     *  4. Check folder /usr/share/ca-certificates/mozilla if it has this CA certificate. If it does, copy content to
     *     ca-chain.cert.pem, otherwise return to step 2
     *
     * @return operation result
     */
    virtual int connect() = 0;

    /**
     *  Disconnects from WebSocket
     */
    virtual void disconnect() = 0;

    /**
     *  This method calls rpc bellow and puts it together:
     *  system_chain
     *  system_name
     *  system_version
     *  system_properties
     *
     * @return SystemInfo struct with result
     */
    virtual unique_ptr<SystemInfo> getSystemInfo() = 0;

    /**
     *  Retreives the block hash for specific block
     *
     *  @param struct with blockNumber block number
     *  @return BlockHash struct with result
     */
    virtual unique_ptr<BlockHash> getBlockHash(unique_ptr<GetBlockHashParams> params) = 0;

    /**
     *  Retreives the runtime metadata for specific block
     *
     *  @param struct with blockHash 64 diget number in hex format
     *  @return Metadata struct with result
     */
    virtual unique_ptr<Metadata> getMetadata(unique_ptr<GetMetadataParams> params) = 0;

    /**
     *  Retreives the runtime version information for specific block
     *
     * @param struct with blockHash 64 diget number in hex format
     * @return RuntimeVersion struct with result
     */
    virtual unique_ptr<RuntimeVersion> getRuntimeVersion(unique_ptr<GetRuntimeVersionParams> params) = 0;

    /**
     *  Subscribe to most recent block number. Only one subscription at a time is allowed. If a subscription already
     * exists, old subscription will be discarded and replaced with the new one. Until unsubscribeBlockNumber method is
     * called, the API will be receiving updates and forwarding them to subscribed object/function. Only
     * unsubscribeBlockNumber will physically unsubscribe from WebSocket endpoint updates.
     *
     * @param callback - functor or lambda expression that will receive updates
     * @return operation result
     */
    virtual int subscribeBlockNumber(std::function<void(long long)> callback) = 0;

    /**
     *  Unsubscribe from WebSocket endpoint and stop receiving updates with most recent block number.
     *
     * @return operation result
     */
    virtual int unsubscribeBlockNumber() = 0;

    /**
     *  Subscribe to most recent balance for a given address. Only one subscription at a time per address is allowed. If
     * a subscription already exists for the same address, old subscription will be discarded and replaced with the new
     * one. Until unsubscribeBalance method is called with the same address, the API will be receiving updates and
     * forwarding them to subscribed object/function. Only unsubscribeBalance will physically unsubscribe from WebSocket
     * endpoint updates.
     *
     * @param address - address to receive balance updates for
     * @param callback - functor or lambda expression that will receive balance updates
     * @return operation result
     */
    virtual int subscribeBalance(string address, std::function<void(unsigned __int128)> callback) = 0;

    /**
     *  Unsubscribe from WebSocket endpoint and stop receiving updates for address balance.
     *
     * @param address - address to stop receiving balance updates for
     * @return operation result
     */
    virtual int unsubscribeBalance(string address) = 0;

    /**
     *  Subscribe to era and session. Only one subscription at a time is allowed. If a subscription already
     * exists, old subscription will be discarded and replaced with the new one. Until subscribeEraAndSession method is
     * called, the API will be receiving updates and forwarding them to subscribed object/function. Only
     * unsubscribeBlockNumber will physically unsubscribe from WebSocket endpoint updates.
     *
     * @param callback - functor or lambda expression that will receive updates
     * @return operation result
     */
    virtual int subscribeEraAndSession(std::function<void(Era, Session)> callback) = 0;

    /**
     *  Unsubscribe from WebSocket endpoint and stop receiving updates with era and session.
     *
     * @return operation result
     */
    virtual int unsubscribeEraAndSession() = 0;
};