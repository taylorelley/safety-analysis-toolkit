// SPDX-License-Identifier: Apache-2.0
// Thin Pi extension: the engine remains a standard MCP stdio server.
import { Client } from '@modelcontextprotocol/sdk/client/index.js';
import { StdioClientTransport } from '@modelcontextprotocol/sdk/client/stdio.js';
import { Type } from '@sinclair/typebox';
export default function(pi) {
  let client, transport;
  async function connect() {
    if(client)return client;
    if(!process.env.SAFETY_BINARY||!process.env.SAFETY_WORKSPACE)throw new Error('Set SAFETY_BINARY and SAFETY_WORKSPACE');
    transport=new StdioClientTransport({command:process.env.SAFETY_BINARY,args:['mcp','--path',process.env.SAFETY_WORKSPACE],env:{...process.env}});
    client=new Client({name:'pi-safety-extension',version:'1.0.0'});
    await client.connect(transport);
    return client;
  }
  pi.on('session_start',async()=>{
    const mcp=await connect();const inventory=await mcp.listTools();
    for(const tool of inventory.tools)pi.registerTool({name:tool.name.replaceAll('.','_'),label:tool.name,description:tool.description,parameters:Type.Unsafe(tool.inputSchema),
      async execute(_id,args,signal){const result=await mcp.callTool({name:tool.name,arguments:args},undefined,{signal});return {content:result.content,details:result.structuredContent,isError:result.isError};}});
  });
  pi.registerCommand('safety-validate',{description:'Execute configured MCP operations without model inference',handler:async(args)=>{
    const mcp=await connect();const operation=JSON.parse(args||'{"tool":"safety.doctor","arguments":{}}');
    if(!operation.tool.startsWith('safety.'))throw new Error('Only safety operations supported');
    const result=await mcp.callTool({name:operation.tool,arguments:operation.arguments||{}});
    pi.sendMessage({customType:'safety-validation',content:JSON.stringify(result),display:true}, {triggerTurn:false});
  }});
  pi.on('session_shutdown',async()=>{if(client)await client.close();});
}
