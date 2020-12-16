using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

using UnityEngine;

namespace EE.Editor {
    /// <summary>
    /// https://gist.github.com/zcyemi/527269ad89c6ed3f10d5867004781e46
    /// </summary>
    public class GradleConfig {
        private readonly string _filePath;

        public GradleNode Root { get; }

        public GradleConfig(string filePath) {
            var file = File.ReadAllText(filePath);
            TextReader reader = new StringReader(file);

            _filePath = filePath;
            Root = new GradleNode("root");
            var curNode = Root;

            var str = new StringBuilder();

            while (reader.Peek() > 0) {
                var c = (char) reader.Read();
                switch (c) {
                    case '/':
                        if (reader.Peek() == '/') {
                            reader.Read();
                            var comment = reader.ReadLine();
                            curNode.AppendChildNode(new GradleCommentNode(comment, curNode));
                        } else {
                            str.Append('/');
                        }
                        break;
                    case '\n': {
                        var strf = FormatStr(str);
                        if (!string.IsNullOrEmpty(strf)) {
                            curNode.AppendChildNode(new GradleContentNode(strf, curNode));
                        }
                        str = new StringBuilder();
                        break;
                    }
                    case '\r':
                        break;
                    case '\t':
                        break;
                    case '{': {
                        var n = FormatStr(str);
                        if (!string.IsNullOrEmpty(n)) {
                            var node = new GradleNode(n, curNode);
                            curNode.AppendChildNode(node);
                            curNode = node;
                        }
                        str = new StringBuilder();
                        break;
                    }
                    case '}': {
                        var strf = FormatStr(str);
                        if (!string.IsNullOrEmpty(strf)) {
                            curNode.AppendChildNode(new GradleContentNode(strf, curNode));
                        }
                        curNode = curNode.Parent;
                        str = new StringBuilder();
                        break;
                    }
                    default:
                        str.Append(c);
                        break;
                }
            }

            //Debug.Log("Gradle parse done!");
        }

        public void Save(string path = null) {
            path = path ?? _filePath;
            File.WriteAllText(path, Print());
            //Debug.Log("Gradle parse done! " + path);
        }

        private string FormatStr(StringBuilder sb) {
            var str = sb.ToString();
            str = str.Trim();
            return str;
        }

        public string Print() {
            var sb = new StringBuilder();
            PrintNode(sb, Root, -1);
            return sb.ToString();
        }

        private string GetLevelIndent(int level) {
            if (level <= 0) return "";
            var sb = new StringBuilder("");
            for (var i = 0; i < level; i++) {
                sb.Append('\t');
            }
            return sb.ToString();
        }

        private void PrintNode(StringBuilder stringBuilder, GradleNode node, int level) {
            if (node.Parent != null) {
                if (node is GradleCommentNode) {
                    stringBuilder.Append("\n" + GetLevelIndent(level) + @"//" + node.Name);
                } else {
                    stringBuilder.Append("\n" + GetLevelIndent(level) + node.Name);
                }
            }

            if (!(node is GradleContentNode) && !(node is GradleCommentNode)) {
                if (node.Parent != null) {
                    stringBuilder.Append(" {");
                }
                foreach (var c in node.Children) {
                    PrintNode(stringBuilder, c, level + 1);
                }
                if (node.Parent != null) {
                    stringBuilder.Append("\n" + GetLevelIndent(level) + "}");
                }
            }
        }
    }

    public class GradleNode {
        private List<GradleNode> _children = new List<GradleNode>();
        private GradleNode _parent;
        protected string _name;
        public GradleNode Parent => _parent;
        public string Name => _name;
        public List<GradleNode> Children => _children;

        public GradleNode(string name, GradleNode parent = null) {
            _parent = parent;
            _name = name;
        }

        public void Each(Action<GradleNode> f) {
            f(this);
            foreach (var n in _children) {
                n.Each(f);
            }
        }

        public void AppendChildNode(GradleNode node) {
            _children = _children ?? new List<GradleNode>();
            _children.Add(node);
            node._parent = this;
        }

        /// <summary>
        /// 节点路径索引
        /// </summary>
        /// <param name="path"> Sample "android/signingConfigs/release"</param>
        /// <returns></returns>
        public GradleNode TryGetNode(string path) {
            var subPath = path.Split('/');
            var current = this;
            for (var i = 0; i < subPath.Length; i++) {
                var p = subPath[i];
                if (string.IsNullOrEmpty(p)) continue;
                var tnode = current.FindChildNodeByName(p);
                if (tnode == null) {
                    Debug.Log("Can't find Node:" + p);
                    return null;
                }

                current = tnode;
            }

            return current;
        }

        public GradleNode FindChildNodeByName(string name) {
            foreach (var n in _children) {
                if (n is GradleCommentNode || n is GradleContentNode)
                    continue;
                if (n.Name == name)
                    return n;
            }
            return null;
        }

        public bool ReplaceContentStartsWith(string patten, string value) {
            foreach (var n in _children) {
                if (!(n is GradleContentNode)) continue;
                if (n._name.StartsWith(patten)) {
                    n._name = value;
                    return true;
                }
            }
            return false;
        }

        public GradleContentNode ReplaceContentOrAddStartsWith(string patten, string value) {
            foreach (var n in _children) {
                if (!(n is GradleContentNode)) continue;
                if (n._name.StartsWith(patten)) {
                    n._name = value;
                    return (GradleContentNode) n;
                }
            }
            return AppendContentNode(value);
        }

        /// <summary>
        /// 添加子节点
        /// </summary>
        /// <param name="content"></param>
        /// <returns></returns>
        public GradleContentNode AppendContentNode(string content) {
            foreach (var n in _children) {
                if (!(n is GradleContentNode)) continue;
                if (n._name == content) {
                    Debug.Log("GradleContentNode with " + content + " already exists!");
                    return null;
                }
            }
            var cnode = new GradleContentNode(content, this);
            AppendChildNode(cnode);
            return cnode;
        }

        public bool RemoveContentNode(string contentPattern) {
            for (var i = 0; i < _children.Count; i++) {
                if (!(_children[i] is GradleContentNode)) continue;
                if (_children[i]._name.Contains(contentPattern)) {
                    _children.RemoveAt(i);
                    return true;
                }
            }
            return false;
        }
    }

    public sealed class GradleContentNode : GradleNode {
        public GradleContentNode(string content, GradleNode parent) : base(content, parent) {
        }

        public void SetContent(string content) {
            _name = content;
        }
    }

    public sealed class GradleCommentNode : GradleNode {
        public GradleCommentNode(string content, GradleNode parent) : base(content, parent) {
        }

        public string GetComment() {
            return _name;
        }
    }
}